//Conatains methods to check for collision


//int main(int x, int y){


// returns -1 if ball is going out, 1 otherwise
int Check_Out(struct wball wba){
	if( wba.boty > 149){
		return -1;
	}
	return 1;
}

// returns -1 if ball collides with edge of screen, 1 otherwise
int Check_Wall(struct wball wbal){
	if( wbal.leftx < 0){
		return -1;
	}
	if( (wbal.leftx + 4) > 128){
		return -1;
	}
	if( (wbal.boty + 4) < 0){
		return -1;
	}
	/*if( y > 156){ //if ball is this low it is going out
		return -1;
	} */
	return 1;
}

// return -1 if ball collides with paddle, 1 otherwise
int Check_Paddle(struct wball wbal, struct paddle pad){
	if( wbal.boty > 151){
     return 1;
	}
	if( wbal.boty > 154){
		return 1;
	}
  int diff = wbal.leftx - pad.leftx;
  if(diff < -3){ // or -4? Check left paddle boundary
    return 1;
  }
  if(diff > 20){ // check right paddle boundary
    return 1;
  }
	return -1;
}

//Call from a for loop
//return -1 if ball collides with a brick, 1 otherwise
int Check_Brick(struct wball wbal, struct brick brick){
	if(brick.health == 0) return 1; //dont collide with broken bricks

	if(brick.leftx > (wbal.leftx + 3)){ //Check left boundary in x
		return 1;
	}
	if((brick.leftx + 15) < wbal.leftx){ //Check right boundary x
		return 1;
	}
	if((brick.boty + 4) < wbal.boty){ //Check top y
		return 1;
	}
	if(brick.boty > (wbal.boty + 3)){ //Check bottom y
		return 1;
	}
	return -1;
}

//Return -1 if collision is on the side of a brick, 1 otherwise
int Check_Side(struct wball wbal, struct brick brick){ //Call if Check_Brick returns -1
  if((wbal.leftx - brick.leftx) > 1){
    if((wbal.leftx - brick.leftx) < -1){
      return 1;
    }
  }
  return -1;
}
