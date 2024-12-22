# All message format for operation
## Authentication
### Register
```json
{
  "type": "REGISTER_REQUEST",
  "timestamp": 1630000000,
  "payload": {
    "username": "user123",
    "password": "hashed_password"
  }
}
```
```json
{
  "type": "REGISTER_RESPONSE",
  "timestamp": 1630000000,
  "payload": {
    "success": true,
    "message": "Register successfully"
  }
}
```
```json
{
  "type": "REGISTER_RESPONSE",
  "timestamp": 1630000000,
  "payload": {
    "success": false,
    "message": "Username already exists"
  }
}
```
### Login
```json
{
  "type": "LOGIN_REQUEST",
  "timestamp": 1630000000,
  "payload": {
    "username": "user123",
    "password": "hashed_password"
  }
}
```
```json
{
  "type": "LOGIN_RESPONSE",
  "timestamp": 1630000000,
  "payload": {
    "status": "SUCCESS",
    "message": "Login successfully"
  }
}
```
```json
{
  "type": "LOGIN_RESPONSE",
  "timestamp": 1630000000,
  "payload": {
    "status": "FAILED",
    "message": "Username or password is incorrect"
  }
}
```