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
### Send message

**From client to server request**

```json
{
    "type": REQUEST_SD_MSG,
    "room_id": "room_id",
    "message": "message",
    "date": "date",
    "msg_type": TEXT_MSG
}
```
**From server to client response**
```json
{
    "type": RQ_SD_MSG,
    "room_id": room_id,
    "message": "message",
    "date": date,
    "message_id": message_id_in_database,
    "user_id": user_id_that_wrote_message,
    "msg_type": TEXT_MSG 
}
