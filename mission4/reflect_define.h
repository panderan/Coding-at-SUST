#ifndef _REFLECT_DEFINE_
#define _REFLECT_DEFINE_

//ITF           接口类名
//ClassName     子类名(产品类名)
//VAR_NAME      变量名
//FTY_CLASSNAME 工厂类名
//PRDT_CLASS    子类名(产品类名 = ClassName)
//PRDT_NAME     子类标签名(字符串表示)

// 功能类
//
// 反射所需函数名
#define REFLECT_CREATE ref_clone

// 在接口类中定义反射所需的纯虚函数
#define REFLECT_INTERFACE(ITF) public: virtual ITF * REFLECT_CREATE() = 0;

// 在实现类中定义反射所需的虚函数
#define REFLECT_DECLARE(ITF) public: virtual ITF * REFLECT_CREATE();

// 在实现类中实现反射所需的虚函数
#define REFLECT_IMPLEMENT(ITF, PRDT_CLASS) ITF * PRDT_CLASS::REFLECT_CREATE() \
{return (ITF *) new PRDT_CLASS ();}

//工厂类
//
// 相关变量名定义
//      子类名称
#define PRDT_NAME prdt_name
//      子类指针
#define PRDT_PTR prdt_ptr
//      子类名长度
#define PRDT_NAME_LEN 128
//      注册集大小
#define PRDT_REGISTATION_LEN 100
//      定义注册集变量
#define DECLEARE_REGISTATION(FTY_CLASSNAME, ITF) static struct { \
    char PRDT_NAME[PRDT_NAME_LEN] = {0};\
    ITF * PRDT_PTR = NULL; \
} FTY_CLASSNAME##_registry [PRDT_REGISTATION_LEN];\
static int FTY_CLASSNAME##_registry_idx = 0;

// 定义工厂类构造函数
#define REFLECT_FACTORY_CONSTRUCTOR(FTY_CLASSNAME,ITF) public: \
    FTY_CLASSNAME(const char *name, ITF *obj);
// 实现工厂类构造函数
#define REFLECT_FACTORY_CONSTRUCTOR_IMPLEMENT(FTY_CLASSNAME, ITF) \
    FTY_CLASSNAME::FTY_CLASSNAME(const char *name, ITF *obj) { \
        string key(name); \
        strcpy(FTY_CLASSNAME##_registry[FTY_CLASSNAME##_registry_idx].PRDT_NAME, name);\
        FTY_CLASSNAME##_registry[FTY_CLASSNAME##_registry_idx].PRDT_PTR = obj; \
        FTY_CLASSNAME##_registry_idx++;\
    }
// 向注册集注册
#define REGISTER_TO_A_FACTORY(FTY_CLASSNAME, PRDT_CLASS, PRDT_NAME, ITF) \
    PRDT_CLASS reg_##PRDT_CLASS; \
    FTY_CLASSNAME fty_##PRDT_CLASS(PRDT_NAME, (ITF *)&reg_##PRDT_CLASS);
// 在注册集中查找
#define SEARCH_REGISTATION(FTY_CLASSNAME, tgt_str, ITF, ret_obj) \
    for (int iii=0; iii<FTY_CLASSNAME##_registry_idx; iii++) { \
        ret_obj = NULL;\
        if (strcasecmp(tgt_str, FTY_CLASSNAME##_registry[iii].PRDT_NAME) == 0) {\
            ret_obj = (ITF *)FTY_CLASSNAME##_registry[iii].PRDT_PTR->REFLECT_CREATE();\
            break;\
        }\
    }

    

#endif

