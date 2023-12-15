import subprocess
import shutil
import os
import re

# Define paths
app_binary = '/home/karol/Repositories/Modbus-Industrial-Diagnostic-Tool/build/build-Modbus-Industrial-Diagnostic-Tool-Desktop_Qt_6_5_3_GCC_64bit-Release/exe/Modbus-Industrial-Diagnostic-Tool'
qt_base_dir = '/home/karol/Qt/6.5.3/gcc_64'  # Adjust if needed
deployment_dir = '/home/karol/Repositories/Modbus-Industrial-Diagnostic-Tool/build/build-Modbus-Industrial-Diagnostic-Tool-Desktop_Qt_6_5_3_GCC_64bit-Release/exe/'

# Function to copy files and directories
def copy(source, destination):
    if os.path.isdir(source):
        shutil.copytree(source, destination, dirs_exist_ok=True)
    else:
        os.makedirs(os.path.dirname(destination), exist_ok=True)
        shutil.copy(source, destination)

# Function to extract library paths from ldd output
def parse_ldd_output(output):
    libs = []
    for line in output.splitlines():
        match = re.search(r'\s=>\s(/[^ ]+)', line)
        if match:
            libs.append(match.group(1))
    return libs

try:
    # Run ldd to find dependencies
    process = subprocess.run(['ldd', app_binary], stdout=subprocess.PIPE, text=True)
    dependencies = parse_ldd_output(process.stdout)

    # Copy dependencies (Qt libraries only)
    for lib in dependencies:
        if qt_base_dir in lib:
            copy(lib, os.path.join(deployment_dir, os.path.basename(lib)))

    # Example: Copy Qt plugins (adjust based on your application's needs)
    qt_plugins_dir = os.path.join(qt_base_dir, 'plugins')
    for plugin_subdir in ['platforms', 'imageformats']:
        source_dir = os.path.join(qt_plugins_dir, plugin_subdir)
        if os.path.exists(source_dir):
            copy(source_dir, os.path.join(deployment_dir, 'plugins', plugin_subdir))

except Exception as e:
    print(f"Error occurred: {e}")

print("Deployment completed. Files copied to:", deployment_dir)

