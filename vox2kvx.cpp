/*
 * Convert a VOX file to a KVX file
 * 
 * Written by John314
 * 
 * Info found here:
 * https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
 * https://github.com/vuolen/slab6-mirror
 * https://github.com/sirjuddington/SLADE/pull/1063/commits/724f175aa5a652b2c2364e836341dd8422300d2e
 * https://github.com/guillaumechereau/goxel/blob/master/src/formats/voxlap.c
 * 
 * Compie with: g++ vox2kvx.cpp -o vox2kvx
 * 
 * */

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <math.h> 

// ABGR ???
uint32_t default_palette[256] = {
	0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
	0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
	0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
	0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
	0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
	0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
	0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
	0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
	0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
	0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
	0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
	0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
	0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
	0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
	0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
	0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
};

// RGB
uint32_t UsePalette[256];

uint32_t VoxelPalette[257]; // stored as ABGR

uint32_t findCQColor(uint32_t c_index){
	float difmach = 0.01;
	uint32_t out_index = 0x100;
	bool foundMatch = false;
	
	while(!foundMatch && difmach < 1.0f) {
		for(int i = 0; i < 256; i++){
			float rA = ((UsePalette[i]>>16)&0xFF)/255.0f;
			float gA = ((UsePalette[i]>>8)&0xFF)/255.0f;
			float bA = (UsePalette[i]&0xFF)/255.0f;
			// Limit the floating point
			rA = roundf(rA*100.0f)/100.0f;
			gA = roundf(gA*100.0f)/100.0f;
			bA = roundf(bA*100.0f)/100.0f;

			float bB = ((VoxelPalette[c_index]>>16)&0xFF)/255.0f;
			float gB = ((VoxelPalette[c_index]>>8)&0xFF)/255.0f;
			float rB = (VoxelPalette[c_index]&0xFF)/255.0f;
			// Limit the floating point
			rB = roundf(rB*100.0f)/100.0f;
			gB = roundf(gB*100.0f)/100.0f;
			bB = roundf(bB*100.0f)/100.0f;

			float difR = fabsf(rB - rA);
			float difG = fabsf(gB - gA);
			float difB = fabsf(bB - bA);
			
			//float totalDif = (difR + difG + difB) / 3.0f;
			if(difR <= difmach && difG <= difmach && difB <= difmach){
				out_index = i;
				foundMatch = true;
	/*			std::cout << "----------------------------" << std::endl;
				std::cout << "Difference: " << difR << "," << difG << "," << difB << std::endl;
				std::cout << "Color A: " << rA << "," << gA << "," << bA << std::endl;
				std::cout << "Color B: " << rB << "," << gB << "," << bB << std::endl;
				std::cout << "Color switched:" << (int)c_index << " - " << (int)out_index << std::endl;*/
			}
		}
		difmach *= 10.0f;
	}
	if(!foundMatch)
		std::cout << "Warn! Failed to find a close color!" << std::endl;
	return out_index; // Uhhh...
};

struct Voxel {
	int32_t x,y,z, color;
};

std::vector<Voxel> VoxelData;
int VoxelSize[3]; // x, y, z


// MagicaVoxel .VOX format stuff
struct VOX_Header {
	char hash[4]; // Must be VOX_ (_ is a space)
	uint32_t version; // Should be 150
};

struct VOX_Chunk;

struct VOX_Chunk {
	char chunk_id[4]; // Can be MAIN PACK SIZE XYZI RGBA
	uint32_t content_size;
	uint32_t children_size;
	std::vector<uint8_t> content;
	std::vector<VOX_Chunk> children;
};


uint32_t makeInt32(std::vector<uint8_t> data, int offset){
	return data.at(offset) | (data.at(offset+1)<<8) | (data.at(offset+2)<<16) | (data.at(offset+3)<<24);
};


std::string fixVoxName(char *a, int len = 4){
	char b[len+1];
	memcpy(&b, a, len);
	b[len] = 0;
	return std::string(b); // Return the compare
};


void writeChunkInfo(VOX_Chunk &c){
	std::cout << "Chunk Info:" << std::endl;
	std::cout << "Name: " << fixVoxName(c.chunk_id) << std::endl;
	std::cout << "Content Size: " << c.content_size << std::endl;
	std::cout << "Children Size: " << c.children_size << std::endl;

};

bool readChunk(std::ifstream &file, VOX_Chunk &chunk){
	// Read the main "header"
	file.read( (char*)&chunk.chunk_id, sizeof(uint8_t)*4);
	if(file.eof()) return true;
	file.read( (char*)&chunk.content_size, sizeof(uint32_t));
	if(file.eof()) return true;
	file.read( (char*)&chunk.children_size, sizeof(uint32_t));		
	if(file.eof()) return true;

	// Read the data (if any)
	char c;
	for(int i = 0; i < chunk.content_size; i++){
		file.read( (char*)&c, sizeof(uint8_t));		
		if(file.eof()) return true;
		chunk.content.push_back(c);
	}
	
	//writeChunkInfo(chunk);
	return false;
};

void loadVOX(std::string fname){
	std::ifstream file;
	file.open(fname);
	if(!file.is_open()){
		std::cout << "Error opening " << fname << std::endl;
		return;
	}
	VOX_Header temp_head;
	file.read( (char*)&temp_head.hash, sizeof(uint8_t)*4);
	file.read( (char*)&temp_head.version, sizeof(uint32_t));
	if(fixVoxName(temp_head.hash).compare("VOX ")!=0){
		std::cout << fname << " is not a valid VOX format! Header got:" << fixVoxName(temp_head.hash) << std::endl;
		return;
	}
	std::cout << "Loading voxel file " << fname << " ver." << temp_head.version << std::endl;
	
	VOX_Chunk file_chunk;

	// Load the first chunk
	if(readChunk(file, file_chunk)){
		std::cout << "Error reading chunk!" << std::endl;
		file.close();
		return;
	}
	if(fixVoxName(file_chunk.chunk_id).compare("MAIN")!=0){
		std::cout << "No MAIN chunk! Quiting!" << std::endl;
		file.close();
		return;		
	}
	// Write the gathered information
	//writeChunkInfo(file_chunk);	

	// Read all the children
	int readsize = file_chunk.children_size;
	while(readsize){
		VOX_Chunk read_chunk;
		if(readChunk(file, read_chunk)){
			std::cout << "Error reading chunk!" << std::endl;
			file.close();
			return;
		}
		// Add the child to the main chunk
		file_chunk.children.push_back(read_chunk);
		// Write the gathered information
		//writeChunkInfo(read_chunk);
		// Remove bytes read
		readsize -= (4+4+4+read_chunk.content_size);
	}
	// Go through all the chunks
	for(VOX_Chunk &chunk : file_chunk.children){
		// Write the gathered information
		//writeChunkInfo(chunk);
		std::string fixedName = fixVoxName(chunk.chunk_id);
		if(fixedName.compare("SIZE")==0){
			// Get the size of the model
			VoxelSize[0] = makeInt32(chunk.content,0);
			VoxelSize[1] = makeInt32(chunk.content,4);
			VoxelSize[2] = makeInt32(chunk.content,8);
			std::cout << "Model size is: " << VoxelSize[0] << "x" << VoxelSize[1] << "x" << VoxelSize[2] << std::endl;
		}
		if(fixedName.compare("XYZI")==0){
			uint32_t numVoxels = makeInt32(chunk.content,0);
			std::cout << "NumVoxels:" << numVoxels << std::endl;
			for(int i = 0; i < numVoxels; i++){
				Voxel local = {
					chunk.content.at(4+(i*4)),
					chunk.content.at(4+(i*4)+1),
					chunk.content.at(4+(i*4)+2),
					chunk.content.at(4+(i*4)+3)
				};
				VoxelData.push_back(local);
				//std::cout << VoxelData.back().x << "," << VoxelData.back().y << "," << VoxelData.back().z << "," << VoxelData.back().color << std::endl;
			}
		}
		if(fixedName.compare("RGBA")==0){
			for(int i = 0; i < 256; i++){
				VoxelPalette[i+1] = makeInt32(chunk.content,(i*4));
			}
		}
	}

	file.close();
};

#define MAX_SIZE 256

uint8_t slicedData[MAX_SIZE][MAX_SIZE][MAX_SIZE];
uint8_t slicedVisable[MAX_SIZE][MAX_SIZE][MAX_SIZE];
int slicedSize[3];


uint8_t sliceAt(int x,int y,int z){
	if(x < 0 || y < 0 || z < 0) return 255;
	if(x >= slicedSize[0] || y >= slicedSize[1] || z >= slicedSize[2]) return 255;
	return slicedData[x][y][z];
};

uint8_t sliceAtV(int x,int y,int z){
	if(x < 0 || y < 0 || z < 0) return 0;
	if(x >= slicedSize[0] || y >= slicedSize[1] || z >= slicedSize[2]) return 0;
	return slicedVisable[x][y][z];
};


void sliceVoxData(){
	int minsize[3] = { 0 };
	
	// clear all the memory to appropriate values
	memset(&slicedData,255,sizeof(slicedData));
	memset(&slicedVisable,0,sizeof(slicedVisable));
	
	
	for(Voxel &v : VoxelData){
		if(v.x < 0 || v.y < 0 || v.z < 0){
			std::cout << "Error! Bad voxel data!" << std::endl;
			return;
		}
		if(v.x >= MAX_SIZE || v.y >= MAX_SIZE || v.z >= MAX_SIZE){
			std::cout << "Error! Voxel out of range!" << std::endl;
			return;
		}
		slicedData[v.x][v.y][v.z] = findCQColor(v.color);
		if(v.x > minsize[0]){ minsize[0] = v.x; }
		if(v.y > minsize[1]){ minsize[1] = v.y; }
		if(v.z > minsize[2]){ minsize[2] = v.z; }
	}
	slicedSize[0] = minsize[0]+1;
	slicedSize[1] = minsize[1]+1;
	slicedSize[2] = minsize[2]+1;
	
	std::cout << "Optimized size to: " << slicedSize[0] << "x" << slicedSize[1] << "x" << slicedSize[2]  << std::endl;
	
	// Optimize face culling
	for(int sx = 0; sx < slicedSize[0]; sx++){
		for(int sy = 0; sy < slicedSize[1]; sy++){
			for(int sz = 0; sz < slicedSize[2]; sz++){				
				uint8_t backFaceCullInfo = 0; //Low 6 bits tell which of 6 faces are exposed
				if(sliceAt(sx,sy,sz) != 255){
					if(sliceAt(sx-1,sy,sz) == 255) backFaceCullInfo |= 1;
					if(sliceAt(sx+1,sy,sz) == 255) backFaceCullInfo |= 2;
					if(sliceAt(sx,sy-1,sz) == 255) backFaceCullInfo |= 4;
					if(sliceAt(sx,sy+1,sz) == 255) backFaceCullInfo |= 8;
					//if(sliceAt(sx,sy,sz-1) == 255) 
					backFaceCullInfo |= 16;
					//if(sliceAt(sx,sy,sz+1) == 255) 
					backFaceCullInfo |= 32;
				}
				slicedVisable[sx][sy][sz] = backFaceCullInfo;
			}
		}
	}
};


void saveKVX(std::string fname){
	std::ofstream file;
	file.open(fname);
	if(!file.is_open()){
		std::cout << "Error opening " << fname << std::endl;
		return;
	}
//	int MipMapLevels = 1; // ???
//	for(int i = 0; i < MipMapLevels; i++){
	uint32_t numbytes, vsize[3], pivot[3];
	vsize[0] = slicedSize[0]; // x w
	vsize[1] = slicedSize[1]; // y d
	vsize[2] = slicedSize[2]; // z h
	// Offset of voxels (Assumed 0,0,0) shifted 8 bits up for pecision???
	pivot[0] = pivot[1] = pivot[2] = 0;
	
	// Offset the voxels by the Z height
	pivot[0] = vsize[0] << 7; // Move over half
	pivot[1] = vsize[1] << 7; // Move over half
	pivot[2] = vsize[2] << 8; // Move full
	
	uint32_t xoffset[vsize[0]+1];
	uint16_t xyoffset[vsize[0]*(vsize[1]+1)] = { 0 };
		
	// Create valid data
	std::vector<uint8_t> voxdata;
	long voxdataofffset = (vsize[0]+1)*sizeof(uint32_t) + (vsize[0]*(vsize[1]+1))*sizeof(uint16_t);
	
	std::vector<uint8_t> write_colors;

	// Generate data
	xoffset[0] = voxdataofffset;

	for(int sx = 0; sx < vsize[0]; sx++){
		xyoffset[sx*(vsize[1]+1)] = voxdataofffset - xoffset[sx];
		for(int sy = 0; sy < vsize[1]; sy++){
			for(int sz = 0; sz < vsize[2]; sz++){
				uint8_t vis = sliceAtV(sx,sy,sz);
				//std::cout << "VIS: 0x" << std::hex << (int)vis <<std::dec << std::endl;
				if(sliceAt(sx,sy,sz) != 255){
					write_colors.push_back(sliceAt(sx,sy,sz));
				}
				if( !write_colors.empty() && ( (sz == (vsize[2]-1)) || 
												(sliceAt(sx,sy,sz+1)==255 ) ||
												(sliceAtV(sx,sy,sz+1) != vis) ||
												(write_colors.size() == 255) )){ 
/*
					std::cout << "CHECK: " << (int)sliceAt(sx,sy,sz) << " != " << ((int)255) << std::endl;
					std::cout << "CHECK: " << sz << " = " << (vsize[2]-1) << std::endl;
					std::cout << "CHECK: " << (int)sliceAt(sx,sy,sz+1) << " = " << ((int)255) << std::endl;
					std::cout << "CHECK: " << (int)sliceAtV(sx,sy,sz+1) << " != " << (int)(vis) << std::endl;
					std::cout << "CHECK: " << (int)write_colors.size() << " = " << ((int)255) << std::endl;
*/
					// slice header
					char sliceZTop = 0;//vsize[2] - (sz+1);  //Starting z coordinate of top of slab
					char sliceHeight = (char)vsize[2];  //# of bytes in the color array - slab height

					voxdata.push_back(sz-(write_colors.size()-1));
					voxdata.push_back(write_colors.size());
					voxdata.push_back(vis);
//					std::cout << "Writing " << write_colors.size() << " voxels" << std::endl;
					for(int i = 0; i < write_colors.size(); i++){
						voxdata.push_back(write_colors[i]);
					}
					voxdataofffset += (3+write_colors.size());
					write_colors.clear();
				}
			}
			xyoffset[sx*(vsize[1]+1) + (sy+1)] = voxdataofffset - xoffset[sx];
		}
		xoffset[sx+1] = voxdataofffset;
	}
	/*
	for(int i = 0; i < vsize[0]+1; i++){
		std::cout << i << ":" << std::hex << "0x" << (int)xoffset[i] << std::endl;
	}
	std::cout << "------------------------" << std::endl;
	for(int i = 0; i < vsize[0]*(vsize[1]+1); i++){
		std::cout << i << ":" << std::hex << "0x" << (int)xyoffset[i] << std::endl;
	}*/

	numbytes = voxdataofffset + (sizeof(uint32_t)*6);//+sizeof(xoffset)+sizeof(xyoffset));

	
	file.write((char*)&numbytes, sizeof(uint32_t));
	file.write((char*)&vsize, sizeof(uint32_t)*3);
	file.write((char*)&pivot, sizeof(uint32_t)*3);
	 // 0xC 0x10 0x00 0x00 0x04 0x00 0x01 0x3F 0x01
	file.write((char*)&xoffset, sizeof(xoffset));
	file.write((char*)&xyoffset, sizeof(xyoffset));
	
	// Write the actual data
	file.write((char*)voxdata.data(), voxdata.size());
	
	
	// Write the palette info
	for(int i = 0; i < 256; i++){
		uint8_t r,g,b;
//		r = (VoxelPalette[i+1]&0xFF)>>2;
//		g = ((VoxelPalette[i+1]>>8)&0xFF)>>2;
//		b = ((VoxelPalette[i+1]>>16)&0xFF)>>2;
		b = (UsePalette[i]&0xFF)>>2;
		g = ((UsePalette[i]>>8)&0xFF)>>2;
		r = ((UsePalette[i]>>16)&0xFF)>>2;
		file.write((char*)&r, sizeof(uint8_t));
		file.write((char*)&g, sizeof(uint8_t));
		file.write((char*)&b, sizeof(uint8_t));
	}
	
	file.close();
};

uint8_t hexvalue(char value){
	for(int i = 0; i < 16; i++){
		if(tolower(value) == "0123456789abcdef"[i]){
			return i;
		}
	}
	return 0; // ???
};


int main(int argc, char *args[]){
	// Get all the arguments into a vector
	std::vector<std::string> argv;
	for(int i = 0; i < argc; i++){
		argv.push_back(args[i]);
	}
	// Make sure the user is using the app correctly
	if(argv.size() < 3){
		std::cout << "Use: vox2kvx input.vox output.kvx [palette.txt]" << std::endl;
		return 0;
	}
	std::string vox_name = argv.at(1);
	std::string kvx_name = argv.at(2);

	bool loaded_palette = false;

	if(argv.size() > 3){
		// Load the palette file
		std::string palette_name = argv.at(3);
		std::ifstream palette_file(palette_name);
		if(!palette_file.is_open()){
			std::cout << "Error opening palette file: " << palette_name << std::endl;
			std::cout << "Use a valid GIMP exported palette file." << std::endl;
		}else{
			for(int i = 0; i < 256; i++){
				char temp;
				char hexdata[6];
				palette_file.read((char*)&temp, sizeof(uint8_t));
				palette_file.read((char*)&(hexdata[0]), sizeof(uint8_t)*2);
				palette_file.read((char*)&(hexdata[2]), sizeof(uint8_t)*2);
				palette_file.read((char*)&(hexdata[4]), sizeof(uint8_t)*2);
				uint8_t r = (hexvalue(hexdata[0])<<4) | hexvalue(hexdata[1]);
				uint8_t g = (hexvalue(hexdata[2])<<4) | hexvalue(hexdata[3]);
				uint8_t b = (hexvalue(hexdata[4])<<4) | hexvalue(hexdata[5]);
				UsePalette[i] = (r<<16)|(g<<8)|b;
			}
			loaded_palette = true;
		}
	}
	if(loaded_palette == false){
		std::cout << "Using default MagicaVoxel palette." << std::endl;
		for(int i = 0; i < 256; i++){
			uint8_t r = default_palette[i]&0xFF;
			uint8_t g = (default_palette[i]>>8)&0xFF;
			uint8_t b = (default_palette[i]>>16)&0xFF;
			UsePalette[i] = (r<<16)|(g<<8)|b;
		}
	}
	
	// Open the vox file and store it
	loadVOX(vox_name);
	
	// Slice the data into "slices"
	sliceVoxData();
	
	// Open the KVX file and save the voxel data
	saveKVX(kvx_name);
	
	// Quit
	return 0;
};

