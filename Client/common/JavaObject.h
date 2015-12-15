#ifndef JCLASS_H
#define JCLASS_H
/*
 *	�������ļ�����Ŀ¼
 *	javac ���ļ�
 *		����
 *	javap -s ����
 *		�鿴��Ա���ͱ�ǩ
 */
/*
	����java����Ĳ���
	jclass cls = env->FindClass("BackStage/Bar");//ȡ������ 
	jmethodID id = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;)V");//ȡ��Ҫ���õĹ��캯��id
	jobject paramOut = env->NewObject(cls, id1, 1, env->NewStringUTF("create")); //�������󣬵�3��������ʼ���Ǵ��ݸ����캯���Ĳ���

	���ö��󷽷��Ĳ���
	�������󣬲ο����ϲ���
	jmethodID initId = env->GetMethodID(cls, "init", "(ILjava/lang/String;)V");//ȡ�÷���ID
	env->CallObjectMethod(paramOut, initId, 2, env->NewStringUTF("init"));//���÷�������3��������ʼ���Ǵ��ݸ����캯���Ĳ���
*/
#include <jni.h>
#define PATH_SEPARATOR ';'

#define USER_CLASSPATH "." /* where Prog.class is */

namespace jni
{
	//////////////////////////////////////////////////////////////////////////
	//��֤ed
	/*
		jstringתchar*
		char*�ı�����code
		code:UTF-8,GB2312
		�����ַ���������Ҫʹ��free()�ͷ�
	*/
	char* Jstring2Chars(JNIEnv* env, jstring jstr, const char *code);
	/*
		����һ��jstring����
		������str
		������code
		code:UTF-8,GB2312
	*/
	jstring JString( JNIEnv* env, const char *str, const char *code );
	//android Log.d�����־
	void Logcat( JNIEnv* env, const char *filter, const char *format, ... );
	//c++ָ��תjstring
	jstring Pointer2JString(JNIEnv* env, void *pObj);
	//jstringתc++ָ��
	void* JString2Pointer(JNIEnv* env, jstring str);
}

class JavaObject
{
public:
	/*
	 *	����һ��java�����
	 *	classname java��ȫ·��������.����/������"BackStage/Bar"
	 *	signature ���캯����ǩ����ȡ��������
	 *		�������ļ�����Ŀ¼
	 *		���룺javac ���ļ�
	 *		�鿴��Ա���ͱ�ǩ��javap -s ����
	 */
	static jobject NewJObject( JNIEnv *env, const char *classname, const char *signature, ... );
	//����һ��java����󣬲���JavaObject����
	JavaObject(JNIEnv *env, const char *classname, const char *signature, ... );

	//����һ���Ѵ����õĶ���
	JavaObject(JNIEnv* env, jobject pObj, bool bCreateRef = false);
	~JavaObject();
	
	jobject GetObject();//ȡ��java����
	char* GetStringMember( const char *name);
	bool SetStringMember( const char *name, const char *value);
	bool GetIntMember( const char *name, int &value );
	bool SetIntMember( const char *name, int value );
	bool SetPointerMember( const char *name, void *value );
	bool GetPointerMember( const char *name, void *&value );
	/*
	 *	methodname	������
	 *	signature ��Ա���ͱ�ǩ����ȡ��������
	 *		�������ļ�����Ŀ¼
	 *		���룺javac ���ļ�
	 *		�鿴��Ա���ͱ�ǩ��javap -s ����
	 */
	void CallMethod( const char *methodname, const char *signature, ... );
	bool AttachCurrentThread();
	void DetachCurrentThread();
	void DestroyJavaVM();
	
	


protected:

private:
	JavaVM *m_jvm;
	JNIEnv* m_env;
	jclass m_cls;
	bool m_bIsRef;
	jobject m_thisPointer; 
};

#endif //JCLASS_H