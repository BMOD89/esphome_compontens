# D0 component for ESPHome

## About
This external component provides a way to retrieve data from devices using the *D0* format.

A transmitted physical value is identified by an OBIS code (Object Identification System). If it is known which code the manufacturer assigns to the physical value, the corresponding value can be extracted. Unfortunately this differs from manufacturer to manufacturer. Also the transmitted physical values are not fixed.

As an example, many manufacturers use the OBIS code 1-0:1.8.0*255 for the accumulated total active energy.

## Configuration
The communication with the hardware is done using UART. Therefore you need to have an [UART bus](https://esphome.io/components/uart.html#uart) in your configuration with the `rx_pin` connected to the output of your hardware sensor component. The baud rate usually has to be set to 9600bps.

```yaml
# Example configuration entry

external_components:
  - source:
      type: local
      path: components
    components: [ d0 ]

uart:
  id: uart_bus
  tx_pin: GPIO1
  rx_pin: GPIO3
  baud_rate: 9600
  data_bits: 7
  parity: EVEN
  stop_bits: 1

d0:
  id: myd0
  uart_id: uart_bus

sensor:
  - platform: d0
    name: "Total energy"
    d0_id: myd0
    server_id: "0123456789abcdef"
    obis_code: "1-0:1.8.0*255"
    unit_of_measurement: kWh
    accuracy_decimals: 1

  - platform: d0
    name: "Active power"
    d0_id: myd0
    server_id: "0123456789abcdef"
    obis_code: "1-0:15.7.0*255"
    unit_of_measurement: W
    accuracy_decimals: 1

text_sensor:
  - platform: d0
    name: "Manufacturer"
    d0_id: myd0
    server_id: "0123456789abcdef"
    obis_code: "129-129:199.130.3*255"

  - platform: d0
    name: "Total energy text"
    d0_id: myd0
    server_id: "0123456789abcdef"
    obis_code: "1-0:1.8.0*255"
```

## Configuration variables

### D0 platform:
- **id** (*Optional*): Specify the ID used for code generation.
- **uart_id** (*Optional*): Manually specify the ID of the [UART Component](https://esphome.io/components/uart.html#uart).

### Sensor
- **obis_code** (*Required*, string): Specify the OBIS code you want to retrieve data for from the device. The format must be (A-B:C.D.E\*F, e.g. 1-0:1.8.0\*255)
- **server_id** (*Optional*, string): Specify the device's server_id to retrieve the OBIS code from. Should be specified if more then one device is connected to the same hardware sensor component.
- **d0_id** (*Optional*): Specify the ID used for code generation.
- All other options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).

### Text Sensor
- **obis_code** (*Required*, string): Specify the OBIS code you want to retrieve data for from the device. The format must be (A-B:C.D.E\*F, e.g. 1-0:1.8.0\*255)
- **server_id** (*Optional*, string): Specify the device's server_id to retrieve the OBIS code from. Should be specified if more then one device is connected to the same hardware sensor component.
- **d0_id** (*Optional*): Specify the ID used for code generation.
- All other options from [Sensor](https://esphome.io/components/sensor/index.html#config-sensor).


## Obtaining OBIS codes
The physical values in the transmitted D0 telegram are identified by the *OBIS codes*.

In order to get the available OBIS codes provided by your smart meter, simply set up the
D0 platform and observe the log output (the log level must be set to at least ``debug``!).

Each line in the output represents a combination of the OBIS code and the transmitted value.

## Precision errors
Many smart meters emit very huge numbers for certain OBIS codes (like the accumulated total active energy). This may lead to precision errors for the values reported by the sensor component to ESPHome. This shows in the fact that slightly wrong numbers may be reported to HomeAssistant. This is a result from internal limitations in ESPHome and has nothing to do with the D0 component.

If you cannot live with this, you can use the `TextSensor` to transmit the value as a string to HomeAssistant. On the HomeAssistant side you can define a [Template Sensor](https://www.home-assistant.io/integrations/template/) to cast the value into the appropriate format and do some scaling.

For ESPHome we have:
```yaml
# ESPHome configuration file
text_sensor:
  - platform: d0
    name: "Total energy string"
    obis_code: "1-0:1.8.0*255"
```

And in HomeAssistant:
```yaml
# Home Assistant configuration.yaml
template:
  - sensor:
      - name: "Total Energy Consumption"
        unit_of_measurement: "kWh"
        state: >
          {% if states('sensor.total_energy_string') == 'unavailable' %}
            {{ states('sensor.total_energy_consumption') }}
          {% else %}
            {{ ((states('sensor.total_energy_string') | float) * 0.0001) | round(2) }}
          {% endif %}
```

Usually the template sensor's value would turn to 0 if the ESP device is unavailable. This results in problems when using the sensor in combination with the [Utility Meter](https://www.home-assistant.io/integrations/utility_meter/) integration.
The state template provided above checks for the sensor's availability and keeps the current state in case of unavailability.
