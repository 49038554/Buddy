#ifndef JCLASS_H
#define JCLASS_H
/*
 *	进入类文件所在目录
 *	javac 类文件
 *		编译
 *	javap -s 类名
 *		查看成员类型标签
 */
/*
	创建java对象的步骤
	jclass cls = env->FindClass("BackStage/Bar");//取得类型 
	jmethodID id = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;)V");//取得要调用的构造函数id
	jobject paramOut = env->NewObject(cls, id1, 1, env->NewStringUTF("create")); //创建对象，第3个参数开始，是传递给构造函数的参数

	调用对象方法的步骤
	创建对象，参考以上步骤
	jmethodID initId = env->GetMethodID(cls, "init", "(ILjava/lang/String;)V");//取得方法ID
	env->CallObjectMethod(paramOut, initId, 2, env->NewStringUTF("init"));//调用方法，第3个参数开始，是传递给构造函数的参数
*/
#include <jni.h>
#define PATH_SEPARATOR ';'

#define USER_CLASSPATH "." /* where Prog.class is */

namespace jni
{
	//////////////////////////////////////////////////////////////////////////
	//验证ed
	/*
		jstring转char*
		char*的编码是code
		code:UTF-8,GB2312
		返回字符串用完需要使用free()释放
	*/
	char* Jstring2Chars(JNIEnv* env, jstring jstr, const char *code);
	/*
		创建一个jstring对象
		内容是str
		编码是code
		code:UTF-8,GB2312
	*/
	jstring JString( JNIEnv* env, const char *str, const char *code );
	//android Log.d输出日志
	void Logcat( JNIEnv* env, const char *filter, const char *format, ... );
	//c++指针转jstring
	jstring Pointer2JString(JNIEnv* env, void *pObj);
	//jstring转c++指针
	void* JString2Pointer(JNIEnv* env, jstring str);
}

class JavaObject
{
public:
	/*
	 *	创建一个java类对象
	 *	classname java类全路径名，将.换成/，例如"BackStage/Bar"
	 *	signature 构造函数标签，获取方法如下
	 *		进入类文件所在目录
	 *		编译：javac 类文件
	 *		查看成员类型标签：javap -s 类名
	 */
	static jobject NewJObject( JNIEnv *env, const char *classname, const char *signature, ... );
	//创建一个java类对象，并与JavaObject关联
	JavaObject(JNIEnv *env, const char *classname, const char *signature, ... );

	//关联一个已创建好的对象
	JavaObject(JNIEnv* env, jobject pObj, bool bCreateRef = false);
	~JavaObject();
	
	jobject GetObject();//取得java对象
	char* GetStringMember( const char *name);
	bool SetStringMember( const char *name, const char *value);
	bool GetIntMember( const char *name, int &value );
	bool SetIntMember( const char *name, int value );
	bool SetPointerMember( const char *name, void *value );
	bool GetPointerMember( const char *name, void *&value );
	/*
	 *	methodname	方法名
	 *	signature 成员类型标签，获取方法如下
	 *		进入类文件所在目录
	 *		编译：javac 类文件
	 *		查看成员类型标签：javap -s 类名
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