// cons-app.cpp

#include "cons-app.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ConsApp");

namespace ns3 {
namespace ndn{

NS_OBJECT_ENSURE_REGISTERED(ConsApp);

// register NS-3 type
TypeId
ConsApp::GetTypeId()
{
  static TypeId tid = TypeId("ConsApp")
    .SetParent<ndn::App>()
    .AddConstructor<ConsApp>()
    .AddAttribute("Frequency", "Frequency of interest packets", StringValue("1.0"),
                  MakeDoubleAccessor(&ConsApp::m_frequency), MakeDoubleChecker<double>())
    .AddAttribute("Randomize", 
                  "Type of send time randomization: none (default), uniform, exponential",
                  StringValue("none"),
                  MakeStringAccessor(&ConsApp::SetRandomize, &ConsApp::GetRandomize),
                  MakeStringChecker())
    .AddAttribute("Name",
                  "Name of interest",
                  StringValue("/"),
                  MakeStringAccessor(&ConsApp::m_interestName), MakeStringChecker())
    .AddAttribute("AutoAppend",
                  "Auto append sequence number: true (default), false",
                  StringValue("true"),
                  MakeStringAccessor(&ConsApp::m_autoappend), MakeStringChecker())
    ;

  return tid;
}

ConsApp::ConsApp()
  : m_frequency(1.0)
  , m_firstTime(true)
  , m_interestName("/")
  , m_autoappend("true")
{
  m_lastSeq = -1;
}

ConsApp::~ConsApp()
{
} 


void
ConsApp::ScheduleNextPacket()
{
  // double mean = 8.0 * m_payloadSize / m_desiredRate.GetBitRate ();
  // std::cout << "next: " << Simulator::Now().ToDouble(Time::S) + mean << "s\n";

  if (m_firstTime) {
    m_sendEvent = Simulator::Schedule(Seconds(0.0), &ConsApp::SendInterest, this);
    m_firstTime = false;
  }
  else if (!m_sendEvent.IsRunning())
    m_sendEvent = Simulator::Schedule((m_random == 0) ? Seconds(1.0 / m_frequency)
                                                      : Seconds(m_random->GetValue()),
                                      &ConsApp::SendInterest, this);
}

void
ConsApp::SetRandomize(const std::string& value)
{
  if (value == "uniform") {
    m_random = CreateObject<UniformRandomVariable>();
    m_random->SetAttribute("Min", DoubleValue(0.0));
    m_random->SetAttribute("Max", DoubleValue(2 * 1.0 / m_frequency));
  }
  else if (value == "exponential") {
    m_random = CreateObject<ExponentialRandomVariable>();
    m_random->SetAttribute("Mean", DoubleValue(1.0 / m_frequency));
    m_random->SetAttribute("Bound", DoubleValue(50 * 1.0 / m_frequency));
  }
  else
    m_random = 0;

  m_randomType = value;
}

std::string
ConsApp::GetRandomize() const
{
  return m_randomType;
}



// Processing upon start of the application
void
ConsApp::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  ndn::FibHelper::AddRoute(GetNode(), "/edu/ucla/cs/alicelovecpp", m_face, 0);

  std::cout << "Interest Name is " << m_interestName << std::endl;


  ScheduleNextPacket();
}

// Processing when application is stopped
void
ConsApp::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
ConsApp::SendInterest()
{


  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////
  std::string interest_copy = m_interestName;

  // if auto append is true
  if (!m_autoappend.compare("true")){
    interest_copy += "/" + std::to_string(m_lastSeq + 1);
    m_lastSeq += 1;
  }
  

  // Create and configure ndn::Interest
  auto interest = std::make_shared<ndn::Interest>(interest_copy);
  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setInterestLifetime(ndn::time::seconds(1));

  std::cout << "Sending Interest packet for " << interest->getName().toUri() << std::endl;

  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);

  m_appLink->onReceiveInterest(*interest);

  ScheduleNextPacket();
}

// Callback that will be called when Data arrives
void
ConsApp::OnData(std::shared_ptr<const ndn::Data> data)
{
  std::cout << "DATA received for name " << data->getName() << std::endl;
}

} // namespace ndn
} // namespace ns3
