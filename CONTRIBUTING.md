# 开发规范

## 开发环境

使用 Clang 编译器和 C11 标准

## 代码规范

缩进宽度统一为 2 个空格，Tab 按 2 个空格显示。建议开启 `-Wall -Wextra -Wpedantic` 等警告，并使用统一的 Makefile 或构建命令，保证所有成员使用相同的编译方式。

左花括号不换行，例如 `if (...) {`；二元运算符两侧留空格，逗号后留空格；指针声明中的 `*` 跟随变量名，例如 `char *str`；避免一行声明多个变量。普通变量建议使用 `snake_case`，宏和常量使用 `UPPER_SNAKE_CASE`，结构体类型使用 `PascalCase`。

C 代码需要重视错误处理和资源管理。文件打开、内存申请等可能失败的操作必须检查返回值；`malloc`、`calloc` 等申请的内存需要对应释放，`fopen` 打开的文件需要对应 `fclose`。避免随意使用 magic number，应定义有意义的常量或宏。注释主要解释设计原因、特殊情况和不明显的逻辑，不重复描述代码本身。

项目对外或跨文件使用的自定义函数应使用统一的项目或模块前缀，类似 OpenGL 的命名方式，通过前缀避免与其他库或模块的函数发生命名冲突。例如项目缩写为 `CDB` 时，可以使用 `CDBLoadFile()`、`CDBCreateStudent()`、`CDBSaveRecord()`。不同模块可以继续在前缀后体现功能含义。

仅在单个 `.c` 文件内部使用的辅助函数应声明为 `static`，避免向外暴露符号。使用 `camelCase` 命名，不加前缀。`static` 函数如果需要修改结构体对象需要以 `Mut` 结尾，并标注修改的字段。

## Git 规范

项目使用 Git 进行版本管理，源码采用 UTF-8 编码、LF 换行。

Git 提交遵循 Conventional Commits，例如 `feat:`、`fix:`、`refactor:`、`docs:`、`style:`、`test:`、`build:`、`chore:`。提交信息必须由提交者本人手写，不允许直接使用 AI 或其他工具自动生成。

一个 commit 尽量只完成一件事情。已经推送到远程的提交不得通过 `force push`、`rebase`、`amend`、`reset` 等方式重写历史；需要撤销修改时使用新的 commit 或 `git revert`。

所有开发工作在独立分支进行，原则上使用 `feature/xxx`、`fix/xxx`、`refactor/xxx` 等命名，不直接在 `main` 上开发。`main` 设置分支保护，禁止直接 push 和 force push。功能完成后通过 Pull Request 合并到主分支，每个 PR 应保持单一职责，说明修改内容、影响范围和验证方法，并至少经过 1 名其他成员 Review。Review 重点检查功能逻辑、边界条件、错误处理、资源管理、代码结构和可维护性；存在必须解决的问题时应修改后再合并。团队统一采用一种 PR 合并方式。

## 文件组织

项目文件按照模块组织，例如使用 `src/` 存放源文件、`include/` 存放头文件、`tests/` 存放测试、`docs/` 存放文档。一个模块原则上对应一组 `.c` 和 `.h` 文件；仅模块内部使用的函数声明为 `static`；头文件使用 include guard。避免不必要的全局变量和模块之间的强耦合。

仓库应配置 `.gitignore`，不提交编译产物、临时文件、IDE 缓存以及本地环境文件。README 至少说明项目用途、依赖环境、编译方法、运行方法和目录结构。较明确的功能或问题可以通过 Issue 记录，形成 `Issue → Branch → Commit → PR → Review → Merge` 的开发流程。

## AI 使用

AI 仅作为辅助工具使用，可用于解释报错、查询知识、讨论实现思路或辅助检查代码。不得直接提交自己没有理解的 AI 生成代码，每位成员必须能够解释自己提交的实现和设计；Git 提交信息必须由本人编写，Code Review 也不应完全交给 AI 完成。
