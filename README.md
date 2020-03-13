## Hikari
Hikari is a physically-based offline rendering engine, which I write in my spare time to learn what goes into building a production rendering engine.

Some notable features of Hikari are (see gallery below to see them in action)-
- Cosine-weighted and emitter importance sampling
- Area Lights, Spotlights, Directional Lights and Point Lights
- Solid Angle Sampling for Spherical Area Lights which drastically reduce variance as compared to Area Sampling (see below for a comparison)
- Direct Lighting only and Path Tracing integrators
- Matte materials via Perfect Diffuse (Lambertian) and Oren-Nayar BRDF
- Analytical Spheres and Triangle Meshes via `.obj` files
- Mutlithreading via TBB

## Gallery
_Note: Following images are rendered in `.ppm` but were converted to `.png` for uploading._

### Emitters
Spherical Area Light vs Spot Light vs Point Light

<img align="left" src="https://user-images.githubusercontent.com/35342732/76598894-d0d66f00-6529-11ea-8961-52efb7554479.png" width="31%"/>
<img align="left" src="https://user-images.githubusercontent.com/35342732/76603981-e94b8700-6533-11ea-836f-8a8739d9f03b.png" width="31%"/>
<img align="left" src="https://user-images.githubusercontent.com/35342732/76599545-4abb2800-652b-11ea-93c7-472c74786fe3.png" width="31%"/><br/><br/><br/><br/><br/><br/><br/><br/>
<br/><br/><br/><br/>
<br/>

Solid Angle Sampling vs Area Sampling of Spherical Emitters, both images are rendered with 4spp in under 3 seconds on an Intel Core i5-7267U with two cores

<img align="left" src="https://user-images.githubusercontent.com/35342732/76604846-7c38f100-6535-11ea-8ef8-953400955717.png" width="47%"/>
<img align="right" src="https://user-images.githubusercontent.com/35342732/76604870-89ee7680-6535-11ea-9208-1100eceae42f.png" width="47%"/><br/><br/><br/><br/><br/><br/><br/><br/>
<br/><br/><br/><br/>
<br/><br/><br/><br/><br/><br/>

### BRDFs
Perfect Diffuse (Lambertian, standard deviation = 0) vs Oren-Nayar (standard deviation = 90)

<img align="left" src="https://user-images.githubusercontent.com/35342732/76608171-3848ea80-653b-11ea-8c6b-fde1188eb7f8.png" width="47%"/>
<img align="right" src="https://user-images.githubusercontent.com/35342732/76608191-3f6ff880-653b-11ea-86d2-edf78b2bd19a.png" width="47%"/><br/><br/><br/><br/><br/><br/><br/><br/>
<br/><br/><br/><br/>
<br/><br/><br/><br/><br/><br/>

### Integrators
Path Tracing vs Direct Lighting only

<img align="left" src="https://user-images.githubusercontent.com/35342732/76608574-df2d8680-653b-11ea-97d0-2be5efdb05ef.png" width="47%"/>
<img align="right" src="https://user-images.githubusercontent.com/35342732/76608593-e6549480-653b-11ea-8ad6-b6a1853a9897.png" width="47%"/>
