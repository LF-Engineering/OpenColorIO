// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.

#include <Python.h>
#include <sstream>
#include <OpenColorIO/OpenColorIO.h>

#include "PyUtil.h"
#include "PyDoc.h"

namespace OCIO_NAMESPACE
{
namespace
{
PyOCIO_Transform * PyTransform_New(ConstTransformRcPtr transform)
{
    if (!transform)
    {
        return 0x0;
    }

    PyOCIO_Transform * pyobj = 0x0;

    if(ConstAllocationTransformRcPtr allocationTransform = \
        DynamicPtrCast<const AllocationTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_AllocationTransformType);
    }
    else if(ConstCDLTransformRcPtr cdlTransform = \
        DynamicPtrCast<const CDLTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_CDLTransformType);
    }
    else if(ConstColorSpaceTransformRcPtr colorSpaceTransform = \
        DynamicPtrCast<const ColorSpaceTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_ColorSpaceTransformType);
    }
    else if(ConstDisplayTransformRcPtr displayTransform = \
        DynamicPtrCast<const DisplayTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_DisplayTransformType);
    }
    else if(ConstExponentTransformRcPtr exponentTransform = \
        DynamicPtrCast<const ExponentTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_ExponentTransformType);
    }
    else if(ConstFileTransformRcPtr fileTransform = \
        DynamicPtrCast<const FileTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_FileTransformType);
    }
    else if(ConstGroupTransformRcPtr groupTransform = \
        DynamicPtrCast<const GroupTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_GroupTransformType);
    }
    else if(ConstLogTransformRcPtr logTransform = \
        DynamicPtrCast<const LogTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_LogTransformType);
    }
    else if(ConstLookTransformRcPtr lookTransform = \
        DynamicPtrCast<const LookTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_LookTransformType);
    }
    else if(ConstMatrixTransformRcPtr matrixTransform = \
        DynamicPtrCast<const MatrixTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_MatrixTransformType);
    }
    else if(ConstRangeTransformRcPtr rangeTransform = \
        DynamicPtrCast<const RangeTransform>(transform))
    {
        pyobj = PyObject_New(PyOCIO_Transform,
            (PyTypeObject * ) &PyOCIO_RangeTransformType);
    }

    return pyobj;
}
}

PyObject * BuildConstPyTransform(ConstTransformRcPtr transform)
{
    if (!transform)
    {
        Py_RETURN_NONE;
    }

    PyOCIO_Transform * pyobj = PyTransform_New(transform);

    if(!pyobj)
    {
        std::ostringstream os;
        os << "Unknown transform type for BuildConstPyTransform.";
        throw Exception(os.str().c_str());
    }

    pyobj->constcppobj = new ConstTransformRcPtr();
    pyobj->cppobj = new TransformRcPtr();

    *pyobj->constcppobj = transform;
    pyobj->isconst = true;

    return (PyObject *) pyobj;
}

PyObject * BuildEditablePyTransform(TransformRcPtr transform)
{
    if (!transform)
    {
        Py_RETURN_NONE;
    }

    PyOCIO_Transform * pyobj = PyTransform_New(transform);

    pyobj->constcppobj = new ConstTransformRcPtr();
    pyobj->cppobj = new TransformRcPtr();

    *pyobj->cppobj = transform;
    pyobj->isconst = false;

    return (PyObject *) pyobj;
}

bool IsPyTransform(PyObject * pyobject)
{
    return IsPyOCIOType(pyobject, PyOCIO_TransformType);
}

bool IsPyTransformEditable(PyObject * pyobject)
{
    return IsPyEditable<PyOCIO_Transform>(pyobject, PyOCIO_TransformType);
}

TransformRcPtr GetEditableTransform(PyObject * pyobject)
{
    return GetEditablePyOCIO<PyOCIO_Transform, TransformRcPtr>(pyobject,
        PyOCIO_TransformType);
}

ConstTransformRcPtr GetConstTransform(PyObject * pyobject, bool allowCast)
{
    return GetConstPyOCIO<PyOCIO_Transform, ConstTransformRcPtr>(pyobject,
        PyOCIO_TransformType, allowCast);
}

namespace
{

///////////////////////////////////////////////////////////////////////
///

int PyOCIO_Transform_init(PyOCIO_Transform * self, PyObject * args, PyObject * kwds);
void PyOCIO_Transform_delete(PyOCIO_Transform * self);
PyObject * PyOCIO_Transform_str(PyObject * self);
PyObject * PyOCIO_Transform_isEditable(PyObject * self, PyObject *);
PyObject * PyOCIO_Transform_createEditableCopy(PyObject * self, PyObject *);
PyObject * PyOCIO_Transform_getDirection(PyObject * self, PyObject *);
PyObject * PyOCIO_Transform_setDirection(PyObject * self,PyObject * args);

///////////////////////////////////////////////////////////////////////
///

PyMethodDef PyOCIO_Transform_methods[] = {
    { "isEditable",
    (PyCFunction) PyOCIO_Transform_isEditable, METH_NOARGS, TRANSFORM_ISEDITABLE__DOC__ },
    { "createEditableCopy",
    (PyCFunction) PyOCIO_Transform_createEditableCopy, METH_NOARGS, TRANSFORM_CREATEEDITABLECOPY__DOC__ },
    { "getDirection",
    (PyCFunction) PyOCIO_Transform_getDirection, METH_NOARGS, TRANSFORM_GETDIRECTION__DOC__ },
    { "setDirection",
    PyOCIO_Transform_setDirection, METH_VARARGS, TRANSFORM_SETDIRECTION__DOC__ },
    { NULL, NULL, 0, NULL }
};

}

///////////////////////////////////////////////////////////////////////////
///

PyTypeObject PyOCIO_TransformType = {
    PyVarObject_HEAD_INIT(NULL, 0)              //ob_size
    OCIO_PYTHON_NAMESPACE(Transform),           //tp_name
    sizeof(PyOCIO_Transform),                   //tp_basicsize
    0,                                          //tp_itemsize
    (destructor) PyOCIO_Transform_delete,       //tp_dealloc
    0,                                          //tp_print
    0,                                          //tp_getattr
    0,                                          //tp_setattr
    0,                                          //tp_compare
    0,                                          //tp_repr
    0,                                          //tp_as_number
    0,                                          //tp_as_sequence
    0,                                          //tp_as_mapping
    0,                                          //tp_hash 
    0,                                          //tp_call
    (reprfunc)PyOCIO_Transform_str,             //tp_str
    0,                                          //tp_getattro
    0,                                          //tp_setattro
    0,                                          //tp_as_buffer
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   //tp_flags
    TRANSFORM__DOC__,                           //tp_doc 
    0,                                          //tp_traverse 
    0,                                          //tp_clear 
    0,                                          //tp_richcompare 
    0,                                          //tp_weaklistoffset 
    0,                                          //tp_iter 
    0,                                          //tp_iternext 
    PyOCIO_Transform_methods,                   //tp_methods 
    0,                                          //tp_members 
    0,                                          //tp_getset 
    0,                                          //tp_base 
    0,                                          //tp_dict 
    0,                                          //tp_descr_get 
    0,                                          //tp_descr_set 
    0,                                          //tp_dictoffset 
    (initproc) PyOCIO_Transform_init,           //tp_init 
    0,                                          //tp_alloc 
    0,                                          //tp_new 
    0,                                          //tp_free
    0,                                          //tp_is_gc
};

namespace
{

    ///////////////////////////////////////////////////////////////////////
    ///

    int PyOCIO_Transform_init(PyOCIO_Transform * self, PyObject * /*args*/, PyObject * /*kwds*/)
    {
        self->constcppobj = new ConstTransformRcPtr();
        self->cppobj = new TransformRcPtr();
        self->isconst = true;
        std::string message = "Base Transforms class can not be instantiated.";
        PyErr_SetString( PyExc_RuntimeError, message.c_str() );
        return -1;
    }

    void PyOCIO_Transform_delete(PyOCIO_Transform *self)
    {
        DeletePyObject<PyOCIO_Transform>(self);
    }

    PyObject * PyOCIO_Transform_str(PyObject * self)
    {
        OCIO_PYTRY_ENTER()
        ConstTransformRcPtr transform = GetConstTransform(self, true);
        std::ostringstream out;
        out << *transform;
        return PyString_FromString(out.str().c_str());
        OCIO_PYTRY_EXIT(NULL)
    }

    PyObject * PyOCIO_Transform_isEditable(PyObject * self, PyObject *)
    {
        return PyBool_FromLong(IsPyTransformEditable(self));
    }

    PyObject * PyOCIO_Transform_createEditableCopy(PyObject * self, PyObject *)
    {
        OCIO_PYTRY_ENTER()
        ConstTransformRcPtr transform = GetConstTransform(self, true);
        TransformRcPtr copy = transform->createEditableCopy();
        PyOCIO_Transform * pycopy = PyTransform_New(copy);
        pycopy->constcppobj = new ConstTransformRcPtr();
        pycopy->cppobj = new TransformRcPtr();
        *pycopy->cppobj = copy;
        pycopy->isconst = false;
        return (PyObject *) pycopy;
        OCIO_PYTRY_EXIT(NULL)
    }

    PyObject * PyOCIO_Transform_getDirection(PyObject * self, PyObject *)
    {
        OCIO_PYTRY_ENTER()
        ConstTransformRcPtr transform = GetConstTransform(self, true);
        TransformDirection dir = transform->getDirection();
        return PyString_FromString(TransformDirectionToString(dir));
        OCIO_PYTRY_EXIT(NULL)
    }

    PyObject * PyOCIO_Transform_setDirection(PyObject * self, PyObject * args)
    {
        OCIO_PYTRY_ENTER()
        TransformDirection dir;
        if (!PyArg_ParseTuple(args, "O&:setDirection",
            ConvertPyObjectToTransformDirection, &dir)) return NULL;
        TransformRcPtr transform = GetEditableTransform(self);
        transform->setDirection(dir);
        Py_RETURN_NONE;
        OCIO_PYTRY_EXIT(NULL)
    }

}

} // namespace OCIO_NAMESPACE