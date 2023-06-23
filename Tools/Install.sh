# Install Compiler Tools
COMPILER_DEPS="git wget cmake g++"

# Backward Deps
BACKWARD_DEPS="binutils-dev libunwind-dev libdwarf-dev libdw-dev"

# evpp Dependencies
EVPP_DEPS="libevent-dev libgoogle-glog-dev"

# Install Everything
INSTALL_COMMAND="sudo apt install $EVPP_DEPS $BACKWARD_DEPS $COMPILER_DEPS -y"
echo "Running Install Command: $INSTALL_COMMAND"
$INSTALL_COMMAND