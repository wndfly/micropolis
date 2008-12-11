# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.31
#
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _tileengine
import new
new_instancemethod = new.instancemethod
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


TILEENGINE_VERSION = _tileengine.TILEENGINE_VERSION
class TileEngine(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TileEngine, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TileEngine, name)
    __repr__ = _swig_repr
    __swig_setmethods__["width"] = _tileengine.TileEngine_width_set
    __swig_getmethods__["width"] = _tileengine.TileEngine_width_get
    if _newclass:width = _swig_property(_tileengine.TileEngine_width_get, _tileengine.TileEngine_width_set)
    __swig_setmethods__["height"] = _tileengine.TileEngine_height_set
    __swig_getmethods__["height"] = _tileengine.TileEngine_height_get
    if _newclass:height = _swig_property(_tileengine.TileEngine_height_get, _tileengine.TileEngine_height_set)
    __swig_setmethods__["bufData"] = _tileengine.TileEngine_bufData_set
    __swig_getmethods__["bufData"] = _tileengine.TileEngine_bufData_get
    if _newclass:bufData = _swig_property(_tileengine.TileEngine_bufData_get, _tileengine.TileEngine_bufData_set)
    __swig_setmethods__["colBytes"] = _tileengine.TileEngine_colBytes_set
    __swig_getmethods__["colBytes"] = _tileengine.TileEngine_colBytes_get
    if _newclass:colBytes = _swig_property(_tileengine.TileEngine_colBytes_get, _tileengine.TileEngine_colBytes_set)
    __swig_setmethods__["rowBytes"] = _tileengine.TileEngine_rowBytes_set
    __swig_getmethods__["rowBytes"] = _tileengine.TileEngine_rowBytes_get
    if _newclass:rowBytes = _swig_property(_tileengine.TileEngine_rowBytes_get, _tileengine.TileEngine_rowBytes_set)
    __swig_setmethods__["typeCode"] = _tileengine.TileEngine_typeCode_set
    __swig_getmethods__["typeCode"] = _tileengine.TileEngine_typeCode_get
    if _newclass:typeCode = _swig_property(_tileengine.TileEngine_typeCode_get, _tileengine.TileEngine_typeCode_set)
    __swig_setmethods__["floatOffset"] = _tileengine.TileEngine_floatOffset_set
    __swig_getmethods__["floatOffset"] = _tileengine.TileEngine_floatOffset_get
    if _newclass:floatOffset = _swig_property(_tileengine.TileEngine_floatOffset_get, _tileengine.TileEngine_floatOffset_set)
    __swig_setmethods__["floatScale"] = _tileengine.TileEngine_floatScale_set
    __swig_getmethods__["floatScale"] = _tileengine.TileEngine_floatScale_get
    if _newclass:floatScale = _swig_property(_tileengine.TileEngine_floatScale_get, _tileengine.TileEngine_floatScale_set)
    __swig_setmethods__["tileShift"] = _tileengine.TileEngine_tileShift_set
    __swig_getmethods__["tileShift"] = _tileengine.TileEngine_tileShift_get
    if _newclass:tileShift = _swig_property(_tileengine.TileEngine_tileShift_get, _tileengine.TileEngine_tileShift_set)
    __swig_setmethods__["tileMask"] = _tileengine.TileEngine_tileMask_set
    __swig_getmethods__["tileMask"] = _tileengine.TileEngine_tileMask_get
    if _newclass:tileMask = _swig_property(_tileengine.TileEngine_tileMask_get, _tileengine.TileEngine_tileMask_set)
    def __init__(self, *args): 
        this = _tileengine.new_TileEngine(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _tileengine.delete_TileEngine
    __del__ = lambda self : None;
    def setBuffer(*args): return _tileengine.TileEngine_setBuffer(*args)
    def getValue(*args): return _tileengine.TileEngine_getValue(*args)
    def renderTiles(*args): return _tileengine.TileEngine_renderTiles(*args)
    def renderTilesLazy(*args): return _tileengine.TileEngine_renderTilesLazy(*args)
    def renderPixels(*args): return _tileengine.TileEngine_renderPixels(*args)
    def getTileData(*args): return _tileengine.TileEngine_getTileData(*args)
TileEngine_swigregister = _tileengine.TileEngine_swigregister
TileEngine_swigregister(TileEngine)
cvar = _tileengine.cvar



