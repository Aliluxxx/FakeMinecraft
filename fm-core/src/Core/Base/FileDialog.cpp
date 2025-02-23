#include "fmpch.h"

#include <nfd.h>

#include "FileDialog.h"

namespace fm {

	std::filesystem::path FileDialog::OpenFile(const FileDialogSpecification& spec) {

		NFD_Init();

		nfdfilteritem_t* filters = (nfdfilteritem_t*) CreateFilters(spec.FilterList);

		nfdchar_t* outPath;
		std::filesystem::path path;
		nfdresult_t result = NFD_OpenDialog(
			&outPath,
			filters,
			(nfdfiltersize_t)spec.FilterList.size(),
			spec.DefaultPath.empty() ? NULL : spec.DefaultPath.string().c_str()
		);

		if (result == NFD_OKAY) {

			path = outPath;
			NFD_FreePath(outPath);
		}

		else if (result == NFD_CANCEL)
			path = std::filesystem::path("");

		else
			FM_CORE_ASSERT(false, NFD_GetError());

		delete[] filters;
		NFD_Quit();

		return path;
	}

	std::filesystem::path FileDialog::SaveFile(const FileDialogSpecification& spec) {

		NFD_Init();

		nfdfilteritem_t* filters = (nfdfilteritem_t*)CreateFilters(spec.FilterList);

		nfdchar_t* savePath;
		std::filesystem::path path;
		nfdresult_t result = NFD_SaveDialog(
			&savePath,
			filters,
			(nfdfiltersize_t)spec.FilterList.size(),
			spec.DefaultPath.empty() ? NULL : spec.DefaultPath.string().c_str(),
			spec.DefaultFile.empty() ? NULL : spec.DefaultFile.string().c_str()
		);

		if (result == NFD_OKAY) {

			path = savePath;
			NFD_FreePath(savePath);
		}

		else if (result == NFD_CANCEL)
			path = std::filesystem::path("");

		else
			FM_CORE_ASSERT(false, NFD_GetError());

		delete[] filters;
		NFD_Quit();

		return path;
	}

	std::filesystem::path FileDialog::PickFolder(const FileDialogSpecification& spec) {

		NFD_Init();

		nfdchar_t* outPath;
		std::filesystem::path path;
		nfdresult_t result = NFD_PickFolder(
			&outPath,
			spec.DefaultPath.empty() ? NULL : spec.DefaultPath.string().c_str()
		);

		if (result == NFD_OKAY) {

			path = outPath;
			NFD_FreePath(outPath);
		}

		else if (result == NFD_CANCEL)
			path = std::filesystem::path("");

		else
			FM_CORE_ASSERT(false, NFD_GetError());

		NFD_Quit();

		return path;
	}

	void* FileDialog::CreateFilters(const std::vector<FileDialogFilter>& filters) {

		if (filters.empty())
			return nullptr;

		nfdfilteritem_t* filter = new nfdfilteritem_t[filters.size()];
		for (std::size_t i = 0; i < filters.size(); i++) {

			auto& f = filters[i];
			filter[i].name = (nfdu8char_t*)f.Name.c_str();
			filter[i].spec = f.Ext.c_str();
		}

		return filter;
	}
}
