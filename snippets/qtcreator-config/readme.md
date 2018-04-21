## Editor theme

- Put `xmonokai2.xml` to `qtcreator/share/qtcreator/styles`
- Text Editor -> Font & Colors
  - Font: Source Code Pro
  - Theme: XMonokai2
- Text Editor -> Display
  - Highlight current line

## Interface theme

- Environment -> Interface
  - Select "Flat Dark" theme

## Google search shortcut

- Environment -> External Tools
  - Add Tool
    - Name: GoogleThis
    - Executable: xdg-open
    - Arguments: https://www.google.com/search?q=%{CurrentDocument:Selection}
    - Output: Ignore
    - Error output: Show in Pane
- Environment -> Keyboard
  - Change shortcut for GoogleThis to Ctrl+Alt+Shift+G

## Build

- Build & Run -> General
  - Open Compile Output pane when building
