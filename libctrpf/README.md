# CTRPluginFramework
**CTRPluginFramework** is a framework to build plugins for Nintendo 3DS games in *3GX* format.

## Building
The latest version of devkitARM and libctru is needed to build the framework. You can run `make dist-bin` in the `Library` folder to generate a *tar* file containing the library and include files.

## Installing & Updating
The installation and updating is done through `(dkp-)pacman`.

### Adding package database
The first time you install any *ThePixellizerOSS* software, you will need to add the package database to `pacman`. The following steps only need to be done once. If the package database was added in the past, you can skip this section.

#### Windows
1. Navigate to `C:/devkitPro/msys2` and run `msys2_shell.bat`.
2. Copy and paste the following command and press Enter.
```sh
if ! grep -Fxq "[thepixellizeross-lib]" /etc/pacman.conf; then echo -e "\n[thepixellizeross-lib]\nServer = https://thepixellizeross.gitlab.io/packages/any\nSigLevel = Optional" | tee -a /etc/pacman.conf > /dev/null; fi; if ! grep -Fxq "[thepixellizeross-win]" /etc/pacman.conf; then echo -e "\n[thepixellizeross-win]\nServer = https://thepixellizeross.gitlab.io/packages/x86_64/win\nSigLevel = Optional" | tee -a /etc/pacman.conf > /dev/null; fi
```
3. Run `pacman -Sy` and verify it mentions the *ThePixellizerOSS* databases.

#### Linux (dkp-pacman)
The following steps have only been tested for debian based installations (such as Ubuntu).

1. Open a command prompt and type the following command.
```sh
if ! grep -Fxq "[thepixellizeross-lib]" /opt/devkitpro/pacman/etc/pacman.conf; then echo -e "\n[thepixellizeross-lib]\nServer = https://thepixellizeross.gitlab.io/packages/any\nSigLevel = Optional" | sudo tee -a /opt/devkitpro/pacman/etc/pacman.conf > /dev/null; fi; if ! grep -Fxq "[thepixellizeross-linux]" /opt/devkitpro/pacman/etc/pacman.conf; then echo -e "\n[thepixellizeross-linux]\nServer = https://thepixellizeross.gitlab.io/packages/x86_64/linux\nSigLevel = Optional" | sudo tee -a /opt/devkitpro/pacman/etc/pacman.conf > /dev/null; fi
```
2. Run `dkp-pacman -Sy` and verify it mentions the *ThePixellizerOSS* databases.

#### Linux (system pacman / Arch)

1. Open a command prompt and type the following command.
```sh
if ! grep -Fxq "[thepixellizeross-lib]" /etc/pacman.conf; then echo -e "\n[thepixellizeross-lib]\nServer = https://thepixellizeross.gitlab.io/packages/any\nSigLevel = Optional" | sudo tee -a /etc/pacman.conf > /dev/null; fi; if ! grep -Fxq "[thepixellizeross-linux]" /etc/pacman.conf; then echo -e "\n[thepixellizeross-linux]\nServer = https://thepixellizeross.gitlab.io/packages/x86_64/linux\nSigLevel = Optional" | sudo tee -a /etc/pacman.conf > /dev/null; fi
```
2. Run `pacman -Sy` and verify it mentions the *ThePixellizerOSS* databases.

### Installing
The following steps are required to install **CTRPluginFramework**.

#### Windows
1. Navigate to `C:/devkitPro/msys2` and run `msys2_shell.bat`.
2. If you haven't ran `pacman -Sy` recently, run it now, or the installation may fail.
3. Run `pacman -S libctrpf` to install the package.

#### Linux
1. Open a command prompt.
2. If you haven't ran `pacman -Sy` recently, run it now, or the installation may fail.
3. Run `dkp-pacman -S libctrpf` to install the package.
    - Replace `dkp-pacman` with `pacman` in the case of a system pacman installation (eg. Arch).

### Updating
Any updates to **CTRPluginFramework**, alongside other packages can be done with the following commands.

#### Windows
1. Navigate to `C:/devkitPro/msys2` and run `msys2_shell.bat`.
2. Run `pacman -Syu` to check for updates. Make sure to run this command multiple times until it says everything is up to date.

#### Linux
1. Open a command prompt and run `dkp-pacman -Syu` to check for updates.
    - Replace `dkp-pacman` with `pacman` in the case of a system pacman installation (eg. Arch).

## Uninstalling
The following steps are required to remove **CTRPluginFramwork**.

#### Windows
1. Navigate to `C:/devkitPro/msys2` and run `msys2_shell.bat`.
2. Run `pacman -R libctrpf` to remove the package

#### Linux
1. Open a command prompt and run `dkp-pacman -R libctrpf` to remove the package.
    - Replace `dkp-pacman` with `pacman` in the case of a system pacman installation (eg. Arch).

## License
See the [LICENSE file](LICENSE.txt).
