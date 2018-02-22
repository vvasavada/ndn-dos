#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

	int main(int argc, char* argv[]){
		CommandLine cmd;
		cmd.Parse(argc, argv);

		AnnotatedTopologyReader topologyReader("", 1);
		topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-binary-tree.txt");
		topologyReader.Read();

		// Install NDN stack on all nodes
		ndn::StackHelper ndnHelper;
		ndnHelper.InstallAll();

		// Installing global routing interface on all nodes
  		ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  		ndnGlobalRoutingHelper.InstallAll();
		
		// Getting containers for the consumer/producer
  		Ptr<Node> consumers[6] = {Names::Find<Node>("leaf-1"), Names::Find<Node>("leaf-2"),
                            Names::Find<Node>("leaf-3"), Names::Find<Node>("leaf-4"), 
                        	Names::Find<Node>("leaf-5"), Names::Find<Node>("leaf-6")};
  		Ptr<Node> producer = Names::Find<Node>("root");

  		for (int i = 0; i < 6; i++) {
    		ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
    		consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second

    		// Each consumer will express unique interests /root/<leaf-name>/<seq-no>
    		consumerHelper.SetPrefix("/root/" + Names::FindName(consumers[i]));
    		consumerHelper.Install(consumers[i]);
  		}

  		ndn::AppHelper producerHelper("ns3::ndn::Producer");
  		producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

  		// Register /root prefix with global routing controller and
  		// install producer that will satisfy Interests in /root namespace
  		ndnGlobalRoutingHelper.AddOrigins("/root", producer);
  		producerHelper.SetPrefix("/root");
  		producerHelper.Install(producer);

  		// Calculate and install FIBs
  		ndn::GlobalRoutingHelper::CalculateRoutes();
  		
  		Simulator::Stop(Seconds(20.0));

  		Simulator::Run();
  		Simulator::Destroy();

  		return 0;

	}
}

int main(int argc, char *argv[]){
	return ns3::main(argc, argv);
}
