#pragma once 
#include <QString>
#include <QImage>
#include "ExportConfig.hpp"
namespace NF {
	class NOTE_FILE_EXPORT ImageSave {
	public:
		ImageSave();
		~ImageSave();
	public:
		bool save(const QImage& image,QString endName = ".png");
		QString getPath();

	public:
		static void setDefaultFolder(const QString& folder);

	private:
		//检查文件夹是否存在 ，不存在则创建
		void disposFolderExist();
		//获取路径中文件的数量
		int  getFileCount();
		//检查文件是否已经存在
		bool disposFileExist(const QString& fileP);
	private:
		static QString defaultFolder;
		QString date;
		QString path;
		QString fileName;
	};
}