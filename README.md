Run simulation through ns3 folder.

To run the simulation, please follow the following steps:

1) Copy topology text files to ns3/src/ndnSIM/examples/topologies
2) Copy cpp files to ns3/src/ndnSIM/examples/
3) Copy folder with custom apps to ns3/src/ndnSIM/examples/

- Run ./waf configure --enable-examples
- Run ./waf
- Run ./waf --run=*scenario --vis
