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
    "id": id
}
```
___

### Create message

**From client to server request**

```json
{
    "type": CREATE_MSG,
    "room_id": id
}
```
**From server to client response**

```json
{
    "type": CREATE_MSG,
    "msg_id": id
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
    "message_id": msg_id,
    "date": "date",
    "msg_type": TEXT
}
```

**From server to client response**
```json
{
    "type": SND_MSG,
    "username": "username_of_sender",
    "room_id": room_id,
    "message": "message",
    "message_id": msg_id,
    "date": "date",
    "msg_type": TEXT/FILE/IMAGE
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

### Create room 

**From client to server request**

```json
{
    "type": CREATE_GROUP,
    "group_name": "group_name",
    "members": ["member1", "member2", "member3"]
}
```
**From server to client request**

```json
{
    "type": CREATE_GROUP,
    "group_name": "group_name",
    "members": ["member1", "member2", "member3"]
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
    "id": [id1, id2, id3, id4]
}
```
___

### Upload old messages

**From client to server request**

```json
{
    "type": OLD_MSG,
    "id": room_id,
}
```

**From server to client response**
```json
{
    "type": OLD_MSG,
    "messages": ["msg1", "msg2", "msg3"],
    "senders": ["sender1", "sender2", "sender3"],
    "dates": ["time1", "time2", "time3"]
}
```
___