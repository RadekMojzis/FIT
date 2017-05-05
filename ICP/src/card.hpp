#pragma once

class Card{
  private:
		int id;
		bool face_up;
		
  public:
		int get_id(){return id;};
		bool is_up(){return face_up;};
		void flip(){face_up = !face_up;}
};
