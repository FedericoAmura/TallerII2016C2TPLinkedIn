JsonChecker
==========================================

   Archivo que contiene un único método estático, el cual recibe un json, una lista de strings con nombres de campos, y en caso de que falte alguno levanta una excepcion de tipo BadInputException.

   La sintaxis de la funcion en el archivo es interesante, permite que sea utilizada simplemente como f(json, arg1, arg2, ..., argn)

.. cpp:function:: template<typename... Args> void camposExisten(const json11::Json &json, const Args ...args) 

    Chequea que todos los args existan como campos del json.
    Recomiendo no pasar tipos de datos que no sean los especificados a continuacion como argumento.

    :param json: El json a analizar
    :param ...args: Cada uno un const char*
