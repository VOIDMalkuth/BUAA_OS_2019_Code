# BUAA_OS_2019_Code
BUAA Operating System（操作系统，OS）课程设计个人代码

这是本人在北航2019年 *操作系统课程设计（Operating System, OS）*  课程中的代码，谨供各位学弟学妹学习该课程时参考使用。

该代码是个人的实现，大部分内容通过了课程的测试，具体请查看对应result分支里的评测记录。即便是通过的代码，也不能保证没有问题。

## 说明

每个Lab存在于不同分支中，**前面的Lab里可能有错误在后面的Lab中被改正**

exam开头的分支是上机考试的分支（如您所见惨不忍睹）

lab5-challenge\*分支为challenge项目，其中lab5-challenge-alter是通过对gxemul的修改完成的，而lab5-challenge分支是通过更改代码来适应gxemul来实现的，具体可以参考相关文档

部分分支下有UnitTests目录，是本人对代码中某些部分的测试，其中对于链表的测试`test_queue.c`比较有用，而对sched的测试没有太大意义，可以无视。

master分支下有本人的实验报告和有关思考题，可以加以参考。（不保证正确性，同时建议各位同学先进行自己的思考再参考）

## 使用方法

这个README在master分支，请使用形如`git checkout lab6`这样的语句切换到对应分支查看代码。

或者，直接在github上下载lab6分支的代码。

## 一些小建议

~~辱OS课设斗士不请自来~~

当你遇到迷惑问题，找不出自己的问题，可以考虑课程代码实现和自己的实现之间存在冲突的可能（大胆点，很有可能就是祖传代码出错了）

虽说OS课设是出名的迷惑，但是还是希望看到这个repository的同学，先尽力自己思考，用正常的调试手段解决问题。在实在迫不得已的情况下再参考往届代码。毕竟，真的搞懂问题所在以及有关原理后的惊喜是任何成绩都无法比拟的。

**任何迷惑行为的背后，都是其原因的**，细读有关代码，你就会发现问题所在（~~当然，如上文所述，问题不一定出在你写的代码上，整个工具链都可能有奇妙Bug，很有可能是gxemul也错了~~）

建议阅读：gxemul文档及代码，MIPS R3000手册（部分内容与gxemul不符），See Mips Run Linux（基本没用，与前两个都不符）

此外，**大胆忽略祖传~~坑死人不偿命~~注释**，一定范围内可以自由发挥实现。

祝大家OS课设能学的明白，体会到~~通宵~~debug成功后的惊喜。
