/*!
 * @file 
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include<math.h>
#include<assert.h>
#include<stdio.h>
#include"student/student_shader.h"
#include"student/gpu.h"
#include"student/uniforms.h"

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(
    GPUVertexShaderOutput     *const output,
    GPUVertexShaderInput const*const input ,
    GPU                        const gpu   ){
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3) ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret* definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()
	Vec3 *position;
	Vec3 *normal;
	position = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
	normal   = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);
	
	Uniforms const Handle = gpu_getUniformsHandle(gpu);//gpu
	UniformLocation const VMUL = getUniformLocation(gpu, "viewMatrix");
	UniformLocation const PMUL = getUniformLocation(gpu, "projectionMatrix");
	
	Mat4 const*const view = shader_interpretUniformAsMat4(Handle, VMUL);
	Mat4 const*const proj = shader_interpretUniformAsMat4(Handle, PMUL);
	
	//craft final transformation
	
	Mat4 workBench;
  multiply_Mat4_Mat4(&workBench, proj, view);
	
	// transform
	Vec4 vtx_poss;
  copy_Vec3Float_To_Vec4(&vtx_poss, position, 1.f);
	multiply_Mat4_Vec4(&output->gl_Position, &workBench, &vtx_poss);
  
	
	
	Vec3 *out0 = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
	Vec3 *out1 = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);
	
	copy_Vec3(out0, position);
	copy_Vec3(out1, normal);
}

void phong_fragmentShader(
    GPUFragmentShaderOutput     *const output,
    GPUFragmentShaderInput const*const input ,
    GPU                          const gpu   ){
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br> 
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte na čistou zelenou.
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()
	
	output->depth = input->depth;
	init_Vec4(&output->color, 0, 256, 0, 1.0f);
	// -----------------------------------------------------
	//------- Get light and camera
	Uniforms const Handle = gpu_getUniformsHandle(gpu);//gpu
	UniformLocation const LPU  = getUniformLocation(gpu, "lightPosition");
	UniformLocation const CPU = getUniformLocation(gpu, "cameraPosition");
	Vec3 const*const light_poss  = shader_interpretUniformAsVec3(Handle, LPU);
	Vec3 const*const camera_poss = shader_interpretUniformAsVec3(Handle, CPU);
	Vec3 const*const vtx_poss = fs_interpretInputAttributeAsVec3(gpu, input, 0);
	
	Vec3 light;
	Vec3 camera;
	sub_Vec3(&light, light_poss, vtx_poss);
	normalize_Vec3(&light, &light);
	sub_Vec3(&camera, camera_poss, vtx_poss);
	normalize_Vec3(&camera, &camera);
	
	// ------ Get normal
	Vec3 const*const vtx_vect = fs_interpretInputAttributeAsVec3(gpu, input, 1);
	Vec3 vtx_norm;
	normalize_Vec3(&vtx_norm, vtx_vect);
	
	float diffuse = vtx_norm.data[0] * light.data[0] +
									vtx_norm.data[1] * light.data[1] +
									vtx_norm.data[2] * light.data[2];
	if(diffuse > 1)
		diffuse = 0;
	
	
	init_Vec4(&output->color, 0, 1, 0, 0);
	multiply_Vec4_Float(&output->color, &output->color, diffuse);
	
	
	Vec3 R;
	multiply_Vec3_Float(&R, &vtx_norm, 2*diffuse);
	sub_Vec3(&R, &R, &light);
	
	float specular = R.data[0] * camera.data[0] +
									 R.data[1] * camera.data[1] +
									 R.data[2] * camera.data[2];

	if(specular < 0)
		specular = 0;
	if(specular > 1){
		specular = 1;
	}
	// aka specular ** 40
	specular *= specular;
	specular *= specular;
	specular *= specular;
	float spec_to_8 = specular;
	specular *= specular;
	specular *= specular;
	specular *= spec_to_8;

	Vec4 spec_color;
	init_Vec4(&spec_color, 1, 1, 1, 0);
	multiply_Vec4_Float(&spec_color, &spec_color, specular);
	add_Vec4(&output->color, &output->color, &spec_color);
	
	output->color.data[3] = 1.0f;

}

/// @}
