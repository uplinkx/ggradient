# Gradient

See the code in action [here](https://mrcolour.github.io/gradient/)

## Summary:

Gradient is a tool to help autogenerate code that can be dropped into an already existing project to have a particular behavior;
making a particular color gradient. It is ideal in projects that deal with height maps, heat maps, energy levels, and for many other one variable visualizations.
The code can be called to preallocate the values of an array or be called every update or frame. The function takes four parameters, one is the input and the other three are references for where to put R, G, B values.

## Instructions:

<p align="center">
  <img height="500" src="https://github.com/uplinkx/gradient/blob/main/resources/preview_grad.png" />
</p>
<br>

The main part of the application are these ![GitHub Logo](https://github.com/uplinkx/gradient/blob/main/resources/slider.png) sliders that appear on the top half. There are two sliders at the ends whose position cannot be changed (a color gradient is between two colors). The program begins with one slider in the middle that can be moved around with ![GitHub Logo](https://github.com/uplinkx/gradient/blob/main/resources/left_arrow.png) or ![GitHub Logo](https://github.com/uplinkx/gradient/blob/main/resources/right_arrow.png) buttons (hold right click to continuously move them, left for pixel precision) You can also move the sliders by dragging them or tapping anywhere on the gradient. You can add additional sliders with the ![Github Logo](https://github.com/uplinkx/gradient/blob/main/resources/add_slider.png) or delete them with ![Github Logo](https://github.com/uplinkx/gradient/blob/main/resources/del_slider.png).

Deleting will remove the current active slider, unless it is one of the ends. To change the active slider simply click on another slider. The color of the current slider can be changed using the RGB ![Github Logo](https://github.com/uplinkx/gradient/blob/main/resources/h_slider.png) sliders on the bottom left, or by pasting a hex color using ![Github Logo](https://github.com/uplinkx/gradient/blob/main/resources/clipboard.png).

![Github Logo](https://github.com/uplinkx/gradient/blob/main/resources/save.png) will generate code based on the state of the sliders and display it to the terminal.

![Github Logo](https://github.com/uplinkx/gradient/blob/main/resources/help.png) will output a small help section to the terminal.

![Github Logo](https://github.com/uplinkx/gradient/blob/main/resources/resolution.png) changes the granularity of the color gradient. Left clicking will make it more fine but it is more graphically intensive to draw the dynamic gradient (depending on your browser and computer of course). Right clicking on this button will lower the resolution making the changes in color more visible. Note that to achieve this affect in a program you can simply pass spaced out values to the autogenerated function.

## Technology:

This was coded in the span of two weeks using C and just SDL from scratch. I use a handy library I created for myself called [SDLX](https://github.com/MrColour/SDLX) which makes it much easier for me to create such applications.

I purchased the sprites from https://cyangmou.itch.io/pixel-menu-gui-hud-icons-basicset and made a few modification where necessary. I created the question and clipboard sprites as best I could to match the style using Aseprite.

It was ported to the web using Emscripten.

## More Previews:

<p align="center"><img height="350" align="left" src="https://github.com/uplinkx/gradient/blob/main/resources/preview3.png" /></p>
<p align="center"><img height="350" align="left" src="https://github.com/uplinkx/gradient/blob/main/resources/preview4.png" /></p>



<p align="center"><img height="350" align="left" src="https://github.com/uplinkx/gradient/blob/main/resources/preview2.png" /></p>
<p align="center"><img height="350" align="left" src="https://github.com/uplinkx/gradient/blob/main/resources/preview6.png" /></p>



<p align="center"><img height="350" align="left" src="https://github.com/uplinkx/gradient/blob/main/resources/preview5.png" /></p>
