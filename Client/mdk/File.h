#ifndef MDK_FILE_H
#define MDK_FILE_H

#include <string>

namespace mdk
{

class File
{
public:
	File();
	File(const std::string &dir, const std::string &name);
	virtual ~File();

	static bool Exist(const std::string &name);
	static bool CreateDir(const std::string &path);
	enum Error
	{
		success = 0,
		faild = 1,
		opened = 2, 
		unopen = 3,
		refuse = 4,//�ܾ�������һ���Ǵ�ģʽ�����˲����Ƿ�����
		outPos = 5,
		errorParam = 6,
		dirNoExist = 7,
		fileNoExist = 8,
		unfinished = 9,
	};
	enum Action
	{
		read = 0,//���ļ�������ļ����������ʧ�ܣ�����ȡ
		write = 1,//���ļ�������ļ��������򴴽��ļ�������ͷ��д��
		add = 2,//���ļ�������ļ����������ʧ�ܣ������ļ�ĩβ�������
		readWrite = 3,//���ļ�����ִ��д��ǰ�ɶ�ȡ��һ��ִ����д�����������ٶ�ȡ
	};
	enum DataFromat
	{
		txt = 0,//�ı���ʽIO
		assii = 1,//�����Ʒ�ʽIO
	};
	Error Open(const std::string &dir, const std::string &name, Action act, DataFromat fromat);
	Error Open(Action act, DataFromat fromat);
	bool Close();
	Error ToHeader();
	Error ToTail();
	Error Move(int pos);
	bool IsEnd();
	Error Read(void *content, int count);
	Error Write(void *content, int count);

private:
	std::string m_dir;
	std::string m_name;
	FILE *m_fp;
	Action m_act;
	DataFromat m_fromat;
	unsigned int m_pos;
	unsigned int m_size;
	bool m_isEnd;
};

}

#endif //MDK_FILE_H