# Network Chat Protocol

## Message Format
All messages follow this JSON format:
```json
{
  "type": "MESSAGE_TYPE",
  "sender": "username",
  "receiver": "username or room_id",
  "content": "message content",
  "timestamp": 1234567890
}