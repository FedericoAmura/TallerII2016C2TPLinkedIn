Fecha
==========================================

.. cpp:class:: Fecha

   Clase para representar fechas de una forma que permita una facil transicion entre strings y el tipo compacto en 4 bytes. Las fechas como string seran en formato DD/MM/AAAA. Para fecha actual/infinito se tomara el string "current". Las fechas uint32_t son en el siguiente formato:32 bits = 16 bits año, 8 bits mes, 8 bits día. En binario: AAAAAAAA AAAAAAAA MMMMMMMM DDDDDDDD. Para fecha actual/infinito se tomara la fecha 0xffffffff

   Para documentación detallada sobre miembros y métodos, consultar el archivo de header, el cual se encuentra extensamente documentado.
