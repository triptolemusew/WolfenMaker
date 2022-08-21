use sdl2::{
    pixels::PixelFormatEnum,
    render::{Canvas, Texture, TextureAccess, TextureCreator},
    video::{Window, WindowContext},
};

use crate::{MapX, MapY};

// pub const SCREEN_WIDTH: u32 = 800;
// pub const SCREEN_HEIGHT: u32 = 600;

pub struct Graphics<'texture_creator> {
    texture: Texture<'texture_creator>,
}

impl<'texture_creator> Graphics<'texture_creator> {
    pub fn new(texture_creator: &'texture_creator TextureCreator<WindowContext>) -> Self {
        let texture = texture_creator
            .create_texture(
                PixelFormatEnum::RGB24,
                TextureAccess::Static,
                MapX as u32,
                MapY as u32,
            )
            .unwrap();

        Graphics { texture }
    }

    pub fn render(&mut self, buffer: &[u8], canvas: &mut Canvas<Window>) {
        // canvas.clear();
        self.texture
            .update(None, buffer, (MapY * 3) as usize)
            .unwrap();
        canvas.copy(&self.texture, None, None).unwrap();
        canvas.present();
    }
}
