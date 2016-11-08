Follow the conventions guide here: https://cyberbossjhcb.github.io/CyberEngineMkIII/conventions.html

PRs welcome, reference any issues you are tackling in them

Make sure new files are added to BOTH the handmade VS solution and the CMakeLists. If you don't have VS, give commit access to maintainers in your PR and they'll do it for you

Fixes for bugs that passed builds should have new tests written for them that check those conditions, no matter how snowflakey

There should be one test scenario per function of an implemented class (Test written before commit 500 don't follow this). Keep 100% windows debug code coverage as well as cover every code path on POSIX.