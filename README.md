# XCT ⚒ 
> ( a CLI wrapper on Apple's xcodebuild)
XCT's goal is to make xcodebuild look and feel like a real unix command. Running tests using a CLI should be powerful, flexible, and quick. XCT will make you want to run your tests from the CLI.

XcodeBuild does a good job of simply running tests, but often the command is just too long to be viable to use in a practical sense. Also, the CLI is missing crucial features that would give it a leg up from running tests via the Xcode gutter.  

Synopsis
---

```
XCT -t [@saveable fileTarget]
    -esd [testCaseMatcher]
    -P [@saveable project]
    -S [@saveable scheme] 
    -O [@saveable OS]
    -D [@saveable device]
    --dry --ugly --clear-cache
```

Description
---
**Saveable Enviroment Context**

Enviroment Context is the preconditions of your tests, see **Test Environment Context Flags** 

**@saveable** use the '$' token infront of a value to preform a inline save.

> Alternatively, edits to `~/.xctrc` will count as a spot for saveable environment context. It should be noted though that inline enviroment saves have precedence over .xctrc. It's recommended to have the defualt fall back enviroment context in .xctrc and short term enviroment context changes by inline saves.
**inline saves do not override .xctrc**

e.g.
Run a test and inline save the test target

`XCT -t $someViewControllerTests -s someUIButton`

Run a test using that inline saved test target

`XCT -s someUIButton`

or, to simply run the last test

`XCT`



**Tests Matching Flags**

Note: Don't include the first "test" string that is in every XC test.
Every command should contain one of these, but no more than than one.

```diff
- e  Requires extact match
- s  All matching substrings
- d  Default regex matching
```
e.g. 
```
XCT -t $someViewController_Tests -e updatesLayoutFor_HeightChange_Success
XCT -t $someViewController_Tests -s updatesLayoutFor
XCT -t $someViewController_Tests -d success$
```

**Test Environment Context Flags**

```diff
@@ saveable @@
- t Testing file target
- P Xcode project/workspace file name
- S Target scheme
- O OS version
- D Device
```

**Optional Flags**

```diff
--dry List tests, but don't run
--ugly Negate "pretty" flag
--clear-cache removes all saved env flags
```

Vim Friendly
---

Use XCT as a **_bang_** command combined with the **%** token to target the current file.

![Vim Usage](https://github.com/JoeManto/XCT/blob/master/vimUsage.png)
