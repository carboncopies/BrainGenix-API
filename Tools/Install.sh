# Install Compiler Tools
COMPILER_DEPS="git wget cmake g++"

# Backward Deps
BACKWARD_DEPS="binutils-dev libunwind-dev libdwarf-dev libdw-dev"

# # evpp Dependencies
# EVPP_DEPS="libevent-dev libgoogle-glog-dev libboost-dev google-mock libgflags-dev "

# Install Everything
INSTALL_COMMAND="sudo apt install $BACKWARD_DEPS $COMPILER_DEPS -y"
echo "Running Install Command: $INSTALL_COMMAND"
$INSTALL_COMMAND

# Bootstrap vcpkg
echo "Setting Up vcpkg, ensure you have the submodules cloned!"
./../ThirdParty/vcpkg/bootstrap-vcpkg.sh
echo "Done, you can now run Build.sh"
