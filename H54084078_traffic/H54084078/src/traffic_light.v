module traffic_light(
	input  clk,
    	input  rst,
    	input  pass,
    	output reg R,
    	output reg G,
    	output reg Y
);

//write your code here

reg[2:0] state;
reg[9:0] cycles;

always@(posedge clk or posedge rst)
begin
	if(rst==1'd1)
	begin state=3'd0;cycles=10'd0; R=0;G=1;Y=0;end//
	else if(pass==1'd1) 
	begin
		if(state!=3'd0) begin state=3'd0;cycles=10'd0;R=0;G=1;Y=0; end//
		else begin cycles=cycles+10'd1; G=1;end //G always=1
	end
	else begin
	if(state==3'd6&&cycles==10'd1023) begin state=3'd0;cycles=3'd0; R=0;G=1;Y=0;end
	else if(state==3'd0&&cycles==10'd1023)
		begin state=state+3'd1; cycles=10'd0;R=0;G=0;Y=0; end
	else if(state==3'd1&&cycles==10'd127)
		begin state=state+3'd1; cycles=10'd0;R=0;G=1;Y=0; end
	else if(state==3'd2&&cycles==10'd127)
		begin state=state+3'd1; cycles=10'd0;R=0;G=0;Y=0; end
	else if(state==3'd3&&cycles==10'd127)
		begin state=state+3'd1; cycles=10'd0;R=0;G=1;Y=0; end
	else if(state==3'd4&&cycles==10'd127)
		begin state=state+3'd1; cycles=10'd0;R=0;G=0;Y=1; end
	else if(state==3'd5&&cycles==10'd511)
		begin state=state+3'd1; cycles=10'd0;R=1;G=0;Y=0; end 
	else begin cycles=cycles+10'd1; end
end end
endmodule
