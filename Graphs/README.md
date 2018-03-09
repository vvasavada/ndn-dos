Graph 1 (a1): Locally reachable producer prefix is hard to guess and overwhelm

We show that rtr-ucla-cs can be overwhelmed with traffic but it is impossible for attacker to reach ucla-cs-alicelovecpp. Hence, average number of incoming interests per second for ucla-cs-alicelovecpp remains almost constant with time and well under DoS mark (~120 interest/sec) while rtr-ucla-cs remains under DoS.

Graph 2 (b1): Using valid interests to attack globally reachable producer but cache witholds excess traffic

If attacker keeps using certain range of interest prefixes (say /prefix[1-30]), eventually if the cache size is large enough, all the data produced will be locally cached and DoS traffic will be eliminated.

Graph 3 (b2): Prefix aggregation (no caching!)

Graph shows how rtr-ucla-cs aggregates prefix and forwards only single interest packet to ucla-cs-webserver0-index, significantly decreasing the traffic on it.

Graph 4 (b9): Satisfaction Ratio

With more and more fake interests, satisfaction ratio keeps declining. Spikes are created because there are also legitimate consumers sending valid packets.