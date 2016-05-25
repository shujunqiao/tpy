import MyCppModule  as QT
import Sprite

def display(name):
    print "hi",name
    print "abc1"
    print QT.MyCppFun1("231")
    print "abc"
    Sprite.create("HelloWorld.png")