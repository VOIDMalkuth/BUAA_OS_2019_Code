# Design Notes

### queue.h
- **DO NOT** use function call in macros : ~~`LIST_INSERT_AFTER(head, getNode(15), field);`~~

