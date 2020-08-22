# XCT ⚒ 
> ( a CLI wrapper on Apple's xcodebuild)
XCT's goal is to make xcodebuild look and feel like a real unix command. Running tests using a CLI should be powerful, flexible, and quick, but the xcodebuild solution just isn't. XCT will make you want to run your tests from the CLI.

XcodeBuild does a good job of simply running tests, but often the command is just too long to be viable to use in a practical sense. Also, the CLI is missing crucial features that would give it a leg up from running tests in the Xcode gutter.  

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

**@saveable** use the '$' token infront of a value you wish to not enter in the future.

eg. `XCT -t $someViewControllerTests -s someUIButton`
so, the next test is `XCT -s someUIButton`



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

**Test Environment Flags**

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
