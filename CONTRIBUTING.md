# Contributing
Contributions of all types are welcome, be it code, translations or anything else.

## Code style

Use `astyle` to format your source code with the following settings:
```
--style=allman
--max-code-length=160
--indent=spaces=4
--indent-classes
--indent-switches
--indent-namespaces
--break-blocks
--pad-oper
--pad-comma
--pad-header
--unpad-paren
--delete-empty-lines
--align-pointer=type
--align-reference=name
--break-closing-braces
--attach-return-type
--keep-one-line-blocks
```

Use Unix-style paths (use `C:/Users` instead of `C:\\Users`). If you need to pass such a path to a Windows utility, please use `QDir::toNativeSeparators`.
