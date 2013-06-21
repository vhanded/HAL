#include "DeviceRegistry.h"

#include <HAL/Camera/CameraDevice.h>
#include <HAL/IMU/IMUDevice.h>

namespace hal
{

template<typename BaseDevice>
DeviceRegistry<BaseDevice>::DeviceRegistry() {
    RegisterAlias( "bumblebee", "debayer://dc1394:[]://" );
    RegisterAlias( "twizzler",  "deinterlace://v4l://" );
}

template<typename BaseDevice>
DeviceRegistry<BaseDevice>::~DeviceRegistry()
{
}

template<typename BaseDevice>
DeviceRegistry<BaseDevice>& DeviceRegistry<BaseDevice>::I()
{
    static DeviceRegistry<BaseDevice> s_instance;
    return s_instance;
}

// Register factory pointer.
template<typename BaseDevice>
void DeviceRegistry<BaseDevice>::RegisterFactory(
        const std::string& device_name,
        DeviceFactory<BaseDevice>* factory
        )
{
    m_factories[device_name] = factory;
}

template<typename BaseDevice>
void DeviceRegistry<BaseDevice>::RegisterAlias(
        const std::string& name,
        const std::string& alias
        )
{
    m_aliases[name] = alias;
}

template<typename BaseDevice>
std::shared_ptr<BaseDevice> DeviceRegistry<BaseDevice>::Create(const Uri& uri)
{
    // Check for aliases
    std::map<std::string,std::string>::const_iterator iAlias= m_aliases.find( uri.scheme );

    if( iAlias != m_aliases.end() ){
        std::string sAlias = iAlias->second;
        std::ostringstream oss;
        oss << sAlias << uri.url;
        return Create(oss.str());
    }
    else{
        auto pf = m_factories.find(uri.scheme);
        if(pf != m_factories.end()) {
            std::shared_ptr<BaseDevice> dev = pf->second->GetDevice(uri);
//                m_instances[uri.scheme].insert( dev );
            return dev;
        }
        else{
            throw DeviceException("Scheme '" + uri.scheme + "' not registered for factory");
        }
    }
}

template<typename BaseDevice>
void DeviceRegistry<BaseDevice>::Destroy(BaseDevice* /*dev*/)
{
//        auto i = std::find(m_instances.begin(), m_instances.end(), dev);

//        if(i != m_instances.end()) {
//            m_instances.erase(i);
//        }
}

// Explicitly instantiate desired registries.
template class DeviceRegistry<hal::CameraDriverInterface>;
template class DeviceRegistry<hal::IMUDriverInterface>;

}
