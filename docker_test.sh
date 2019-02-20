#! /bin/sh
docker run -ti -v $PWD:/test memory-test:latest bash -c "cd /test/; make DEBUG=yes docker && valgrind --leak-check=full ./main"
