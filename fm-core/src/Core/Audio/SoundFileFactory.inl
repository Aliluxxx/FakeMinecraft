
namespace priv {

	template<typename T>
	std::unique_ptr<SoundFileReader> CreateReader() {

		return std::make_unique<T>();
	}

	template <typename T>
	std::unique_ptr<SoundFileWriter> CreateWriter() {

		return std::make_unique<T>();
	}
}

template<typename T>
void SoundFileFactory::RegisterReader() {

	UnregisterReader<T>();

	ReaderFactory factory{};
	factory.Check = &T::Check;
	factory.Create = &priv::CreateReader<T>;

	m_Readers.push_back(factory);
}

template<typename T>
void SoundFileFactory::UnregisterReader() {

	for (auto it = m_Readers.begin(); it != m_Readers.end();) {

		if (it->Create == &priv::CreateReader<T>)
			it = m_Readers.erase(it);

		else
			it++;
	}
}

template <typename T>
void SoundFileFactory::RegisterWriter() {

	UnregisterWriter<T>();

	WriterFactory factory{};
	factory.Check = &T::Check;
	factory.Create = &priv::CreateWriter<T>;

	m_Writers.push_back(factory);
}

template <typename T>
void SoundFileFactory::UnregisterWriter() {

	for (auto it = m_Writers.begin(); it != m_Writers.end();) {

		if (it->Create == &priv::CreateWriter<T>)
			it = m_Writers.erase(it);
		else
			++it;
	}
}