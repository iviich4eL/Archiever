#! /bin/sh
docker run -ti -v $PWD:/test memory-test:latest bash -c "cd /test/; make main docker && valgrind --leak-check=full ./main"
