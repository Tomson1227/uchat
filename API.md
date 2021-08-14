# API

### General API structure
**requests**
```json
{
    "type": <request_type>,
    "<field_a>": <value_a>,
    "<field_b>": <value_b>,
    "<field_c>": <value_c>,
}
```
**responses**
```json
{
    "type": <response_type>,
    "<field_a>": <value_a>,
    "<field_b>": <value_b>,
    "<field_c>": <value_c>,
}
```
___

### Log in

**From client to server request**

```json
{
    "type": REQUEST_LOGIN,
    "login": "user_login",
    "pass": "user_pass",
}
```
**From server to client response**
```json
{
    "type": RESPONSE_LOGIN,
    "status": LOGIN_OK | LOGIN_WRONG_USER | LOGIN_WRONG_PASS,
}
```
___

### Sign up

**From client to server request**

```json
{
    "type": REQUEST_SIGNUP,
    "login": "user_login",
    "pass": "user_pass",
}
```
**From server to client response**
```json
{
    "type": RESPONSE_SIGNUP,
    "status": SINGUP_OK | SIGNUP_USER_EXIST | SINGUP_FAIL,
}
```
___

### Create room 

**From client to server request**

```json
{
    "type": CREATE_ROOM,
    "username": "username",
    "customer": "customer"    
}
```

**From server to client**

```json
{
    "type": CREATE_ROOM,
    "id": id,
    "customer": "customer"
}
```
___

### Send message

**From client to server request**

```json
{
    "type": SND_MSG,
    "username": "username",
    "room_id": room_id,
    "message": "message",
    "msg_type": TEXT/FILE/IMAGE
}
```

**From server to client response**

```json
{
    "type": SND_MSG,
    "status": STATUS,
    "room_id": room_id,
    "message_id": msg_id,
    "date": "date"
}
```
___

### Read message

**From client to server request**

```json
{
    "type": READ_MSG,
    "read": ALL/RESENT,
    "room_id": room_id
}
```

**From server to client response**
    
```json
{
    "type": READ_MSG,
    "status": STATUS,
    "room_id": room_id,
    "message_id": msg_id,
    "msg_type": TEXT/FILE/IMAGE,
    "message": "message",
    "date": "date",
    "sender": "username"
}
```
___

### Search user

**From client to server request**

```json
{
    "type": SEARCH_USER,
    "user": "username"
}
```

**From server to client response**

```json
{
    "type": SEARCH_USER,
    "user": ["username1", "username2", "username3"]
}
```
___

### Delete room

**From client to server request**

```json
{
    "type": DELETE_ROOM,
    "id": id
}
```

**From server to client response to each room member**
```json
{
    "type": DELETE_ROOM,
    "id": id
}
```
___

### Delete message

**From client to server request**
```json
{
    "type": DELETE_MSG,
    "id": msg_id
}
```

**From server to client response**
```json
{
    "type": DELETE_MSG,
    "id": msg_id
}
```
___

### Edit message

**From client to server request**
```json
{
    "type": EDIT_MSG,
    "id": id_of_msg,
    "new_msg": "new_msg"
}
```
**From server to client request**
```json 
{
    "type": EDIT_MSG,
    "id": id_of_msg,
    "new_msg": "new_msg"
}
```
___

### Upload old dialogs

**From client to server request**

```json
{
    "type": OLD_DIALOGS,
    "username": "username"
}
```

**From server to client response**
```json 
{
    "type": OLD_DIALOGS,
    "dialogs": ["dialog1", "dialog2", "dialog3", "dialog4"],
    "id": [id1, id2, id3, id4],
    "first_massage_id": [massage_id1, massage_id2 ... massage_idn]
}
```
___

