#include "fmpch.h"

#include "Core/Base/FileInputStream.h"
#include "Core/Base/MemoryInputStream.h"
#include "Core/Base/Time.h"
#include "SoundFileFactory.h"
#include "SoundFileReader.h"

#include "InputSoundFile.h"

namespace fm {

	InputSoundFile::StreamDeleter::StreamDeleter(bool theOwned) : Owned{ theOwned } {}

	template <typename T>
	InputSoundFile::StreamDeleter::StreamDeleter(const std::default_delete<T>&) : Owned{ true } {}

	void InputSoundFile::StreamDeleter::operator()(InputStream* ptr) const {

		if (Owned)
			std::default_delete<InputStream>{}(ptr);
	}

	InputSoundFile::InputSoundFile()
		: m_Reader(), m_Stream(nullptr, false), m_SampleOffset(0), m_SampleCount(0), m_SampleRate(0), m_ChannelCount(0)

	{}

	InputSoundFile::~InputSoundFile() {

		Close();
	}

	bool InputSoundFile::OpenFromFile(const std::filesystem::path& path) {

		Close();

		auto reader = SoundFileFactory::CreateReaderFromFilename(path);
		if (!reader)
			return false;

		auto file = std::make_unique<FileInputStream>();

		if (!file->Open(path))
			return false;

		SoundFileReader::Info info{};
		if (!reader->Open(*file, info))
			return false;

		m_Reader = std::move(reader);
		m_Stream = std::move(file);

		m_SampleCount = info.SampleCount;
		m_SampleRate = info.SampleRate;
		m_ChannelCount = info.ChannelCount;

		return true;
	}

	bool InputSoundFile::OpenFromMemory(const void* data, std::size_t size_in_bytes) {

		Close();

		auto reader = SoundFileFactory::CreateReaderFromMemory(data, size_in_bytes);
		if (!reader)
			return false;

		auto memory = std::make_unique<MemoryInputStream>();

		memory->Open(data, size_in_bytes);

		SoundFileReader::Info info{};
		if (!reader->Open(*memory, info))
			return false;

		m_Reader = std::move(reader);
		m_Stream = std::move(memory);

		m_SampleCount = info.SampleCount;
		m_SampleRate = info.SampleRate;
		m_ChannelCount = info.ChannelCount;

		return true;
	}

	bool InputSoundFile::OpenFromStream(InputStream& stream) {

		Close();

		auto reader = SoundFileFactory::CreateReaderFromStream(stream);
		if (!reader)
			return false;

		if (stream.Seek(0) != 0) {

			FM_CORE_ERROR("Failed to open sound file from stream (cannot restart stream)");

			return false;
		}

		SoundFileReader::Info info{};
		if (!reader->Open(stream, info))
			return false;

		m_Reader = std::move(reader);
		m_Stream = { &stream, false };

		m_SampleCount = info.SampleCount;
		m_SampleRate = info.SampleRate;
		m_ChannelCount = info.ChannelCount;

		return true;
	}

	void InputSoundFile::Seek(Uint64 sample_offset) {

		if (m_Reader && m_ChannelCount != 0) {

			m_SampleOffset = std::min(sample_offset / m_ChannelCount * m_ChannelCount, m_SampleCount);
			m_Reader->Seek(m_SampleOffset);
		}
	}

	void InputSoundFile::Seek(Time time_offset) {

		Seek(static_cast<Uint64>(time_offset.AsSeconds() * static_cast<float>(m_SampleRate)) * m_ChannelCount);
	}

	Uint64 InputSoundFile::Read(Int16* samples, Uint64 max_count) {

		Uint64 readSamples = 0;
		if (m_Reader && samples && max_count)
			readSamples = m_Reader->Read(samples, max_count);

		m_SampleOffset += readSamples;

		return readSamples;
	}

	void InputSoundFile::Close() {

		m_Reader.reset();
		m_Stream.reset();
		m_SampleOffset = 0;
		m_SampleCount = 0;
		m_SampleRate = 0;
		m_ChannelCount = 0;
	}

	Time InputSoundFile::GetDuration() const {

		if (m_ChannelCount == 0 || m_SampleRate == 0)
			return Time::Zero;

		return Seconds(
			static_cast<float>(m_SampleCount) / static_cast<float>(m_ChannelCount) / static_cast<float>(m_SampleRate));
	}

	Time InputSoundFile::GetTimeOffset() const {

		if (m_ChannelCount == 0 || m_SampleRate == 0)
			return Time::Zero;

		return Seconds(
			static_cast<float>(m_SampleOffset) / static_cast<float>(m_ChannelCount) / static_cast<float>(m_SampleRate));
	}
}
