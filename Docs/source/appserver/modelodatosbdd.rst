Modelo de datos para la base de datos
==========================================

Se utiliza LevelDB, estamos en un paradigma nosql key-value

----------------
Definiciones
----------------

Toda key sera precedida por un prefijo de typo, que correspondera a un número natural de 8 bits constante único. Se lo notará en ALLCAPS.
uID: UserID, valor autoincremental natural que identifica a un usuario de forma única.
passHash: Hash de 256 bits de la password. Se utilizará SHA-256.

[x] Indica un largo en bytes. Para largo variable dirá "var".

----------------

----------------

LOGIN[1] userName[4] = uid[4] passHash[32]
