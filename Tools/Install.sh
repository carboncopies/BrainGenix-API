# Install Compiler Tools
COMPILER_DEPS="git wget cmake g++"

# Backward Deps
BACKWARD_DEPS="binutils-dev libunwind-dev libdwarf-dev libdw-dev"

# Oatpp-Postgresql Dependencies
OATPP_POSTGRESQL_DEPS="libpostgresql-ocaml-dev"

# Install Everything
INSTALL_COMMAND="sudo apt install $OATPP_POSTGRESQL_DEPS $BACKWARD_DEPS $COMPILER_DEPS -y"
echo "Running Install Command: $INSTALL_COMMAND"
$INSTALL_COMMAND