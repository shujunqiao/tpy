#include "HelloWorldScene.h"
#include "Python.h"

#include "thello.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

//****************************//

// 1 c/cpp中的函数

int my_c_function(const char *arg) {
    
    int n = system(arg);
    CCLOG("n:%d, arg:%s", n, arg);
    return n;
}

// 2 python 包装

static PyObject * wrap_my_c_fun(PyObject *self, PyObject *args) {
    
    const char * command;
    
    int n;
    
    if (!PyArg_ParseTuple(args, "s", &command))//这句是把python的变量args转换成c的变量command
        
        return NULL;
    
    n = my_c_function(command);//调用c的函数
    
    return Py_BuildValue("i", n);//把c的返回值n转换成python的对象
    
}

// 3 方法列表

static PyMethodDef MyCppMethods[] = {
    
    //MyCppFun1是python中注册的函数名，wrap_my_c_fun是函数指针
    
    { "MyCppFun1", wrap_my_c_fun, METH_VARARGS, "Execute a shell command." },
    
    { NULL, NULL, 0, NULL }
    
};

static PyObject * createsp(PyObject *self, PyObject *args) {
    
    const char * command;
    
    Sprite* n;
    
    if (!PyArg_ParseTuple(args, "s", &command))//这句是把python的变量args转换成c的变量command
        
        return NULL;
    
    n = Sprite::create(command);//调用c的函数
    
    n->setPosition(100, 100);
    auto sc = HelloWorld::getLy();
    sc->addChild(n);
    
    return Py_BuildValue("PyObject", n);//把c的返回值n转换成python的对象
    
}



static PyMethodDef SpriteMe[] = {
    { "create", createsp, METH_VARARGS, "create a sprite"},
    {NULL, NULL, 0, NULL}
};
//****************************//

void testpy()
{
    printf("Hello world!\n");
    
    std::string path = FileUtils::getInstance()->fullPathForFilename("python");
    
    char PYTHONHOME[1024];
    strcpy(PYTHONHOME, path.c_str());
    Py_SetPythonHome( PYTHONHOME );
    
    
    Py_Initialize();
    if (!Py_IsInitialized())  return;
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('src/')");
    
    //*******************//
    PyObject *m = Py_InitModule("MyCppModule", MyCppMethods);
    if (m == NULL)
        CCLOG("---init module failed");
    PyObject *m1 = Py_InitModule("Sprite", SpriteMe);
    if (m1 == NULL)
        CCLOG("---init module sprite failed");
    
    //import Module
    PyObject* pModule = PyImport_ImportModule("hello");
    if (!pModule) {
        printf("Can't import Module!/n");
        return;
    }
    
    PyObject* pDict = PyModule_GetDict(pModule);
    if (!pDict) {
        return;
    }
    
    //fetch Function
    PyObject* pFunHi = PyDict_GetItemString(pDict, "display");
    PyObject_CallFunction(pFunHi, "s", "Crazybaby");
    Py_DECREF(pFunHi);
    
    //Release
    Py_DECREF(pModule);
    Py_Finalize();
}

static cocos2d::Layer* ly;

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    ly = Layer::create();
    this->addChild(ly);
    
    testpy();
    
    return true;
}

Layer* HelloWorld::getLy(){
    return ly;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
