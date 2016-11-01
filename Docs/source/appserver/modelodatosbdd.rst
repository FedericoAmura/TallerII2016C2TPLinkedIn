Modelo de datos (DB)
==========================================

Se utiliza LevelDB, estamos en un paradigma nosql key-value

----------------
Definiciones
----------------

Toda key sera precedida por un prefijo de typo, que correspondera a un número natural de 8 bits constante único. Se lo notará en ALLCAPS.

uID: UserID, valor autoincremental natural que identifica a un usuario de forma única.

fID: FotoID, valor autoincremental natural que identifica a una foto de forma única.

passwordHash: Hash de 256 bits de la password que el usuario ingresa. Se espera que el cliente use SHA-256.

Las fechas estaran en un formato de 4 bytes tal que el primer byte será el año, el 3ro el mes, y el 4to el día.

Para fechas "infinitas" se usará el numero máximo de 32 bits en 1.

[x] Indica un largo en bytes. Para largo variable se indicara "VAR".

----------------
Estructura
----------------

**Contador de User IDs**

LAST_UID[1] = uid[4]

**Contador de Foto IDs**

LAST_FID[1] = fid[4]

**Login**

LOG[1] username[VAR] = passwordHash[32] uID[4]

**Datos generales de un usuario** (Longitudes máximas de campos variables en este caso particular será 256 caracteres)

USER_DATA[1] uID[4] = nombreLen[1] nombre[VAR] emailLen[1] email[VAR] ciudadLen[1] ciudad[VAR] fechaNacimiento[4] longitud[8] latitud[8] fID[4]

**Resumen de un usuario**

US_RESUMEN[1] uID[4] = resumen[VAR]

**Foto por fID**

FOTO[1] fID[4] = foto[VAR]

**Thumbnail para una foto por fID**

FOTO_THUMB[1] fID[4] = thumbnail[VAR]

**Puestos de trabajo de un usuario**

US_POS[1] uID[4] = p1_len[1] puesto1[VAR] fecha_inicio[4] fecha_fin[4] p2_len[1] puesto2[VAR] f_inicio[4] f_fin[4] ...

**Skills de un usuario**

US_SKILL[1] uID[4] = s1_len[1] skill1[VAR] s2_len[1] skill2[VAR] ...

**Reverse lookup: Puestos de trabajo** (Lista ordenada por uID)

RL_POS[1] puesto[VAR] = uID1[4 ]uID2[4] ...

**Reverse lookup: Skills** (Lista ordenada por uID)

RL_SKILLS[1] skill[VAR] = uID1[4] uID2[4] ...

**Lista de contactos de un usuario**

US_CONTACT[1] uID[4] = uID1[4] uID2[4] ...

**Lookup: Dos usuarios son contactos** (uID1 < uID2)

US_CONTACT_PAIR[1] uID1[4] uID2[4] = NULL

**Número de contactos de un usuario** (Máximo de 65535 contactos)

US_CONTACT_COUNT[1] uID[4] = len[2]

**Lista de solicitudes de contacto de un usuario**

SOLIC[1] uIDDestino[4] = uIDFuente1[4] uIDFuente2[4] ...

**Numero de solicitudes pendientes**

SOLIC_COUNT[1] uIDDestino[4] = len[2]

**Mensajes de solicitudes de contacto**

SOLIC_TXT[1] uIDFuente[4] uIDDestino[4] = texto[VAR]

**Conteo de popularidad de un usuario**

US_POP[1] uID[4] = count[4]

**Lookup: Un usuario recomendó a otro**

US_POP_PAIR[1] uIDreceptor[4] uIDrecomendador[4] = NULL

**Reverse lookup: Popularidad** (Lista ordenada)

RL_POP[1] = uID1[4] uID2[4] ...

**Locación geográfica**

GEO_US[1] longitud[8] latitud[8] = uID1[4] uID2[4] …

**Conteo de mensajes en una conversación** (uID1 < uID2)

CONV_COUNT[1] uID1[4] uID2[4] = countmsg[4]

**Mensajes particulares de una conversación**

CONV_MSG[1] uID1[4] uID2[4] idmsg[4] = uIDsender[4] msg[VAR]

**Último mensaje leido en una conversacion** (uID1 es el supuesto lector)

CONV_LAST_READ[1] uID1[4] uID2[4] = idmsg[4]

**Conversaciones con mensajes aun no leidos**

CONV_PENDING_READ[1] uid1[4] = uid1[4] uid2[4] …
