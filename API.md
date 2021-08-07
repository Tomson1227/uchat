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
