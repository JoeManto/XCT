# XCT ⚒ 
> ( A preprocessing CLI wrapper on Apple's xcodebuild's test runner)
XCT's goal is to make running tests via a CLI more accessible and practical. Running tests using a CLI should be powerful, flexible, and quick. XCT will make you want to run your tests from the CL.

XcodeBuild does a good job of handling alot of tasks, whether thats compling, achiving, testing etc. But often the commands are just too long to be viable to use in a practical development environment. Which is mostly by design as the scope is focused on helping automation of xcode workspaces and projects.


A Synopsis
---

```swift
XCT -t [@saveable fileTarget]
    -esd [@nullable testCaseMatcher]
    -P [@saveable project]
    -S [@saveable scheme] 
    -O [@saveable OS]
    -D [@saveable device]
    --dry --ugly --clear-inline
    --clear-global --clear-all
    --list-inline --list-global
```

XCT Features
---


1. A productivity focused CL expereince.

    - Short commands 

    - Global/inline enviorment context 

    - Project/workspace specific enviorment context

2. Crucial features that help you get out of the Xcode gutter.

    - Bulk test case running

    - Regex test case matching

    - Direct flags for substring and exact matching.

    - Utilization of xcodebuilds parallelism

    - Xcodebuild flag pass though

Some Quick Examples
---

e.g.
Run a test and inline save the test target

`XCT -t $someViewControllerTests -s someUIButton`

Run a test using that inline saved test target

`XCT -s someUIButton`

or, to simply run the last test

`XCT`


Description
---

**How does XCT work?**

From a top level view XCT will gather global and inline enviroment context saves. Take in your target testing file and make a copy of it and preprocess it based on provided matching input. The preprocessing includes removing comments, and commenting out unmatched test cases. If no matcher is specified (runs whole test suite) or the exact flag is used then no preprocessing is done. 

**Saveable Enviroment Context**

> Enviroment Context is the preconditions for your tests this includes all the of following with the keyword **@saveable** in the synopsis.

You can perform an inline save with the **'$'** token infront of any value with the **@saveable** keyword in the synopsis. These values will be saved for aslong as you want and can be cleared with **--clear-inline** or overwritten with another value by using the same **'$'** keyword. Inline saves are directory specific and act as quick and recommended way of saving re-appliable enviorment context. 

> Alternatively, user specific edits to `~/.xctrc` will count as a spot for saveable environment context. It should be noted though that inline enviroment saves have precedence over .xctrc. It's recommended to have the defualt fall back enviroment context in .xctrc and short term enviroment context changes by inline-saves.
**Inline-saves don't replace changes in .xctrc and .xctrc changes don't override inline-saves**

**Test Matching Flags**

Note: Don't include the first "test" string that is in every XC test.
Every command should contain one of these, but no more than than one.

```swift
- e  Requires extact match
- s  All matching substrings
- d  Default regex matching
```
e.g. 
```swift
XCT -t $someViewController_Tests -e updatesLayoutFor_HeightChange_Success
XCT -t $someViewController_Tests -s updatesLayoutFor
XCT -t $someViewController_Tests -d success$
```

**Test Environment Context Flags**

```swift
  @saveable
- t Testing file target
- P Xcode project/workspace file name
- S Target scheme
- O OS version
- D Device
```

**Optional Flags**

```swift
--dry List tests, but don't run
--ugly Negate "pretty" flag
--clear-cache removes all saved env flags
```

.xctrc editing format
---
You will find that xctrc follows the vimrc editing format pretty closely.

```bash
# Specify the workspace/project
# Every property under will be applied when xcode-select is pointing to this workspace/project
set project Snapchat
# Or
set workspace Snapchat

set targetFile someViewController 
set scheme someXCscheme
set platform iOS Simulator
set device iPhone 8
set os 13.3
# List any optional or xcode passthough flags
--ugly

set project Instagram
...
```

Vim Friendly
---

Use XCT as a **_bang_** command combined with the **%** token to target the current file.

![Vim Usage](https://github.com/JoeManto/XCT/blob/master/vimUsage.png)
