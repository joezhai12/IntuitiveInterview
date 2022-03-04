Joe Zhai Intuitive Interview

To build, navigate to repository root directory:
```bash
./build.sh
```

To run fan control module:
```bash
./build/bin/fan_controller
```
>Note: Executable must be run from root directory and not in another directory due to use of config file.

To modify fan control module configuration, modify `config/fan_configuration.csv`. Be sure to rebuild after to install file into build directory. The file in `build/config/fan_configuration.csv` can be modified as well to avoid re-compiling. Changes will be overwritten with each build, however.

To run test subsystem:
```bash
./build/bin/subsystem #default 1 subsystem
```

To modify number of subsystems:
```bash
./build/bin/subsystem -n <# of subsystems>
```

User is prompted to enter subsystem temperatures, which are then sent to the fan control module.