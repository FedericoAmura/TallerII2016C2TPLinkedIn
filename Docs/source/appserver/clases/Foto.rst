DBRaw
==========================================

.. cpp:class:: DBRaw

   Clase wrapper de levelDB con el modelo de datos propio de la aplicación. Maneja tipos de datos std o propios y devuelve tipos de datos std o propios. Si un input es mayor al tamaño especificado: En caso de strings se los truncara y en caso de datos binarios se levantara una excepción. No esta anotado en cada metodo, pero todos pueden arrojar LevelDBException si la BD falla de alguna forma inesperada.

   Para documentación detallada sobre miembros y métodos, consultar el archivo de header, el cual se encuentra extensamente documentado.
