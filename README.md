# drivers/net_mikrotik

Monitoring libraries for Mikrotik devices

This library allows to query the REST API offered by RouterOS 7 devices, returning
monitoring data about the devices. It is currently rather limited, focussing on the
state of the ethernet interfaces.

## Mikrotik setup

The library works only with RouterOS 7. RouterOS 6 does not have a REST API.
Documentation on how to enable the REST API is
[here](https://help.mikrotik.com/docs/display/ROS/REST+API), but in a nutshell:

- create a user with only the read and api permissions, for instance:

  ~~~
  /user group
  add name=api-readonly policy=api,read
  /user
  add name=syskit-monitoring group=api-readonly password=SOMEPASSWORD
  ~~~

- make sure the www-ssl service is enabled

  ~~~
  /ip service www-ssl disabled=no
  ~~~

- create a self-signed certificate for the www-ssl service if you don't have one
  ~~~
  /certificate
  add name=ca-self-signed-template common-name=ca-self-signed key-usage=key-cert-sign,crl-sign
  add name=router-template common-name=$IP subject-alt-name="IP:$IP
  /certificate
  sign ca-self-signed-template ca-crl-host=$IP name=ca-self-signed
  sign router-template ca=ca-self-signed name=router
  set ca-self-signed trusted=yes
  set router trusted=yes

  /ip service
  set www-ssl certificate=router
  ~~~