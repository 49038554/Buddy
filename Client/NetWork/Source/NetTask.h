#ifndef NETTASK_H
#define NETTASK_H

#include<ctime>

class NetTask
{
public:
	NetTask(void);
	virtual ~NetTask(void);

public:
	//ͨ������
	enum Type
	{
		vs = 0,//��ս
		save = 1,//��������
	};
	short type;
	void *pData;//����
	int dataSize;//pDataָ��buffer������ʵ�ʴ�С
	int spaceSize;//pDataָ��buffer�ռ�
};

#endif //NETTASK_H