use sdl2::{
    pixels::{Color, PixelFormatEnum},
    render::{Canvas, Texture, TextureAccess, TextureCreator},
    video::{Window, WindowContext},
};

use crate::constants::{BASE_HEIGHT, BASE_WIDTH};

pub struct Video<'tex> {
    pub width: u32,
    pub height: u32,
    pub pix_width: u32,
    pub pix_height: u32,
    pub pix_center: u32,
    pub scale: u32,
    pub buffer: Vec<u8>,

    // SDL specific
    texture: Texture<'tex>,
}

impl<'tex> Video<'tex> {
    pub fn new(scale: u32, texture_creator: &'tex TextureCreator<WindowContext>) -> Self {
        let width = BASE_WIDTH * scale;
        let height = BASE_HEIGHT * scale;
        let pix_width = width;
        let pix_height = height;
        let pix_center = pix_height / 2;
        let buffer: Vec<u8> = vec![0; (width * height) as usize * 3];

        let texture = texture_creator
            .create_texture(
                PixelFormatEnum::RGB24,
                TextureAccess::Static,
                BASE_WIDTH,
                BASE_HEIGHT,
            )
            .unwrap();

        Self {
            width,
            height,
            pix_width,
            pix_height,
            pix_center,
            scale,
            buffer,
            texture,
        }
    }

    pub fn put_pixel(&mut self, x: u32, y: u32, color: Color) {
        let offset = (y * self.width + x) as usize * 3;

        self.buffer[offset] = color.r;
        self.buffer[offset + 1] = color.g;
        self.buffer[offset + 2] = color.b;
    }

    pub fn clear_buffer(&mut self) {
        self.buffer = vec![0x66; (self.width * self.height) as usize * 3];
    }

    pub fn present(&mut self, canvas: &mut Canvas<Window>) {
        self.texture
            .update(None, self.buffer.as_ref(), (BASE_WIDTH * 3) as usize)
            .unwrap();
        canvas.copy(&self.texture, None, None).unwrap();
        canvas.present();
    }
}
