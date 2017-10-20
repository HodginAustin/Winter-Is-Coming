# <p align="center"> 2017 CS CAPSTONE PROJECT </p>
## <p align="center"> Winter is Coming... </p>

##### <p align="center"> Prepared for Victor Hsu at Oregon State University </p>

### <p align="center"> Prepared by </p>
###### <p align="center">Austin Hodgin </p>
###### <p align="center">Travis Hodgin </p>
###### <p align="center">Maximillian Schmidt </p>
###### <p align="center">Zach Lerew </p>



Abstract
===================

<span class="nodecor">This project is defined as creating a system that
controls RGB(Red-Green-Blue) LEDs(Light Emitting Diodes) to effectively
control indoor plant growth during the winter season in Oregon. Oregon’s
winters are a hostile environment to grow specialty plants such as
herbs, spices, or foreign plants. The conditions outside will be much
colder, darker, and humid than the summer months. These plants are not
expected to grow well or even survive in such conditions. Bringing the
plants indoors to a more friendly and manageable environment can prove
to be a difficult task. Existing indoor light systems can be expensive,
and difficult to customize and use. Research provided to the project by
the client has shown that some plants grow differently under different
wavelengths(colors) of light. The project will aim to produce a system
that can control the color, intensity, and schedule of a set of RGB LEDs
in a user friendly manner. With custom control over the growing
conditions, the plants will be allowed to grow well, while at the same
time reducing the impact on the user’s busy life. The developed system
will include a microprocessor that will manipulate the color, intensity,
and schedule of the RGB LEDs. Along side this hardware, the development
of an intuitive user interface will allow an end user to interact with
the system with minimal physical interaction. The project is simple at
its core, but many additional features have been developed to increase
functionality and end usability.</span>

Problem Description
===================

During the cold and dark winter months, plant growth becomes difficult
at best, and calamitous at worst. In our state of Oregon we are well
accustomed to overcast, low temperatures, and rain throughout the
winter, spring, and fall months. Our client has a desire to grow herbs
and plants indoors during these long, cold months. Solving this problem
is a desire held by all members of our team. Much like our client, we
enjoy cooking with fresh herbs and vegetables even through the winter.\
\
Many plants and herbs such as tomatoes, basil, ferns, and plants from
foreign climates have an ideal set of environmental conditions that
allow for optimum growth. Variables such as light wavelength, intensity,
temperature, moisture, and pests all have an impact on a plant’s health
and yield. These factors lead many growers to bring their plants indoors
during the winter months.\
\
Even in a climate controlled dwelling, the problem persists. Humans and
plants require very different living conditions. Residents come and go
from work, school, and vacation while plants have evolved to expect the
24hr day-night cycle. Busy humans can leave plants neglected or without
light for days. Current interior plant lighting systems can be expensive
and offer little to no customization. Plant growth systems that do not
adapt to both the requirements of their plants and their gardener’s
schedule lead to frustration, low plant yield, and plant death.

Proposed Solution
=================

The client’s primary concern is solving the problem of interior plant
lighting. The features required to do this are clearly defined in the
*required features* section, but specific implementation details were
left to our team. Each of us have skills in an area related to this
project, and together we have the ability to go above and beyond what
was requested by the client. We have built the set of *additional
features* listed below that we believe compliment the client’s vision
for this project.\
\
Given proper design and documentation, the additional features are
likely to be completed barring any complications. Features that are
especially difficult to accomplish, unlikely to be completed, or are
outside the primary scope of the project are listed as *stretch goals*.

Required features - v1.0
------------------------

-   LED lighting for a single plant bed

    -   A simple LED strip with a single controller attached to a single
        planter

    -   Service running on a micro controller that can control the power
        state of attached LED lights

    -   Configuration settings for the light state is read from a
        configuration file

    -   Changes to the configuration file are recognized and applied by
        the controller

-   Color and intensity Control

    -   User can select the color and light intensity of the light strip

-   Lighting state schedule

    -   User can specify weekly and daily scheduling for the state of
        the light (Color, Intensity, Power)

-   Zoning for individual control over multiple strips

    -   Controller supports individual control of up to 20 zones

    -   Each zone can chain light strips together on one data pin

-   Simple user interface for basic control

    -   Simple interface to edit configuration settings and physically
        transfer changes to the controller

    -   All settings can be changed from this interface, though it may
        not be user friendly

    -   Controller recognizes configuration changes and applies them
        automatically

Additional features - v2.0
--------------------------

-   Improved User Interface

    -   Hosted web interface on local network

    -   Easy to use and responsive interface that shows the current
        state of the system

    -   All system settings can be updated and applied over the network
        without the need for physical access to the controller

-   Sub zoning on an individual light strip

    -   Multiple colors and intensities on a single light strip using
        LED indexing

-   Flexible zoning and sub-zoning

    -   Whole light strips and sub strips can be zoned together for more
        precise lighting control

-   Mobile web interface

    -   Web interface adds mobile support

    -   Android application acting as a wrapper for the web interface

-   Custom enclosure with vertical lighting

    -   Custom designed planter that holds the controller and lights

-   Environmental monitoring

    -   Monitoring for humidity and temperature using additional
        hardware sensors

    -   Web interface plugin to monitor humidity and temperature

Stretch goals - v3.0
--------------------

-   Modular light strips and enclosure

    -   Easy to attach light strips are automatically detected and setup

    -   Snap together enclosures allow quick and effortless system
        control

-   Gardening guide built into interface to help user learn best
    gardening practices

    -   Interface provides tips and suggestions to improve growing and
        gardening

-   Modular planting enclosure with snap together components

    -   Self contained enclosures with plug-and-play connectors for
        automatic and hassle free setup of multiple planters

-   Irrigation system

    -   Hardware and software necessary to facilitate automatic or
        scheduled watering

    -   \*Requires custom enclosure

-   Wireless control over multiple plant growth systems

    -   Ad hoc style wireless system to allow seamless control over
        multiple grow light systems

    -   Support for large distributed systems, such as greenhouses

Performance Metrics
===================

Defining performance metrics for this project has been a back and forth
topic for this team. The client’s requirements are easily met by a team
of four people, but our internal expectations are much higher. To avoid
feature bloat and to guarantee the product functions per the client’s
requirements at a minimum, the features below describe the features
necessary to define the project as a success. Extra features will be
added after the completion of these functionally required features.

-   **All features described by the client** listed in section *Required
    features* are complete.

-   An alternative interface as described in *Additional features*. An
    interface that allows all controller settings to be modified without
    *physical access to the controller*.

    -   This interface will likely be a web server hosted on the
        controller, but is subject to change as the project evolves.
