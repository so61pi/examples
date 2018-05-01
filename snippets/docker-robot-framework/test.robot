*** Settings ***
Documentation          This example demonstrates executing a command on a remote machine
...                    and getting its output.
...
...                    Notice how connections are handled as part of the suite setup and
...                    teardown. This saves some time when executing several test cases.

Library                SSHLibrary
Suite Setup            Open Connection And Log In
Suite Teardown         Close All Connections


*** Variables ***
${HOST}                localhost
${USERNAME}            root
${PASSWORD}            root


*** Test Cases ***
Execute Command And Verify Output
    [Documentation]    Execute Command can be used to run commands on the remote machine.
    ...                The keyword returns the standard output by default.
    ${output}=         Execute Command    echo Hello SSHLibrary!
    Should Be Equal    ${output}          Hello SSHLibrary!


*** Keywords ***
Open Connection And Log In
   Open Connection     ${HOST}
   Login               ${USERNAME}        ${PASSWORD}

