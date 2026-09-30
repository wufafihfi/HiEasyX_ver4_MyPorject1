#pragma once
#include <string>
#include <fstream>
#include <Windows.h>
#include <tchar.h>

#include <json/json.h>

inline std::string GetExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	/*
	strrchr:函数功能：查找一个字符c在另一个字符串str中末次出现的位置（也就是从str的右侧开始查找字符c首次出现的位置），
	并返回这个位置的地址。如果未能找到指定字符，那么函数将返回NULL。
	使用这个地址返回从最后一个字符c到str末尾的字符串。
	*/
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串//
	std::string path = szFilePath;
	return path;
}

// 保存JSON文件
inline bool saveJsonFile(const std::string& filename, const Json::Value& root) {
	std::ofstream file;
	file.open(filename);

	Json::StreamWriterBuilder writerBuilder;
	writerBuilder["indentation"] = "  "; // 缩进2个空格
	writerBuilder["commentStyle"] = "None"; // 不保存注释

	try {
		std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
		writer->write(root, &file);
		file.close();
		return true;
	}
	catch (const std::exception& e) {
		file.close();
		return false;
	}
}

// 读取JSON文件
inline bool readJsonFile(const std::string& filename, Json::Value& root) {
	std::ifstream file;
	file.open(filename, std::ios::binary);
	if (!file.is_open()) { // 失败创建
		saveJsonFile(filename, root);
	}

	Json::CharReaderBuilder readerBuilder;
	std::string parseErrors;

	// 使用二进制模式读取，避免换行符问题
	bool success = Json::parseFromStream(readerBuilder, file, &root, &parseErrors);

	if (!success) {
		// 第2次尝试
		saveJsonFile(filename, root);
		success = Json::parseFromStream(readerBuilder, file, &root, &parseErrors);
		file.close();
		if (!success) {
			return false;
		}
	}

	return true;
}