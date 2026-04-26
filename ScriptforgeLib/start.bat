@echo off
set "ProjectDir=%~1"

:: 用 SAFE 名称！绝对不会被系统解析
set /a "NAMESPACE_RND=%random% * %random% & 0x7FFFFFFF"
set /a "CLASS_RND=%random% * %random% & 0x7FFFFFFF"
set /a "FUNC1_RND=%random% * %random% & 0x7FFFFFFF"
set /a "FUNC2_RND=%random% * %random% & 0x7FFFFFFF"
set /a "FUNC3_RND=%random% * %random% & 0x7FFFFFFF"
set /a "VAR1_RND=%random% * %random% & 0x7FFFFFFF"
set /a "VAR2_RND=%random% * %random% & 0x7FFFFFFF"
set /a "VAR3_RND=%random% * %random% & 0x7FFFFFFF"
set /a "VAR4_RND=%random% * %random% & 0x7FFFFFFF"

:: 输出安全，不会乱码，不会被替换
> "%ProjectDir%random_define.h" (
    echo #define NS_RANDOM    %NAMESPACE_RND%
    echo #define CLS_RANDOM   %CLASS_RND%
    echo #define F1_RANDOM    %FUNC1_RND%
    echo #define F2_RANDOM    %FUNC2_RND%
    echo #define F3_RANDOM    %FUNC3_RND%
    echo #define V1_RANDOM    %VAR1_RND%
    echo #define V2_RANDOM    %VAR2_RND%
    echo #define V3_RANDOM    %VAR3_RND%
    echo #define V4_RANDOM    %VAR4_RND%
)
echo "Please close it to continue..."
exit /b 0