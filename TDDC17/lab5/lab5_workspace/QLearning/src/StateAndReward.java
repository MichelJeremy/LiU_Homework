public class StateAndReward {
	
	private static int DISC_UP = 7;
	private static int DISC_VX = 2;
	private static int DISC_VY = 7;
	/* State discretization function for the angle controller */
	public static String getStateAngle(double angle, double vx, double vy) {
		// discretize angle, only angle matters here since goal is to face up
		int discAngle = discretize(angle, 15, -3.0, 3.0);
		String state = "A=" + discAngle;
		System.out.println("State: " + state);
		return state;
	}

	/* Reward function for the angle controller */
	public static double getRewardAngle(double angle, double vx, double vy) {
		
		double reward = 0;
		int discAngle = discretize(angle, 15, -3.0, 3.0); // needs disc value there
		// the closer to face up, the better the reward
		int diff = Math.abs(discAngle - DISC_UP);

		System.out.println(diff);
		switch(diff) {
			case 0: // at the top
					reward = 200;
					break;
			case 1: //almost face up
					reward = 50;
					break;
			case 2: // a bit more far away from face up (typically, top right-right or top-left-left)
					reward = 30;
					break;
			case 3: // ^
					reward = 15;
					break;
			case 4: // ^
					reward = 5;
					break;
			case 5: // ^
					reward = 2;
					break;
			case 6: // ^
					reward = 1;
					break;
		}
		
		System.out.println("Case : " + discAngle + "Reward: " + reward);
		return reward;
	}

	/* State discretization function for the full hover controller */
	public static String getStateHover(double angle, double vx, double vy) {
		// discretize everything, get them into new variables
		// vx and vy matter now, angle must be face up too
		int discAngle = discretize(angle, 15, -3.0, 3.0);
		int discVx = discretize(vx, 5, -10, 10);
		int discVy = discretize(vy, 15, -10, 10);
		
		String state = "A=" + discAngle + "|VX=" + discVx + "|VY=" + discVy;
		//System.out.println("State: " + state);
		return state;
	}

	/* Reward function for the full hover controller */
	public static double getRewardHover(double angle, double vx, double vy) {

		
		int discAngle = discretize(angle, 15, -3.0, 3.0);
		int discVx = discretize(vx, 5, -10, 10);
		int discVy = discretize(vy, 15, -10, 10);
		
		int diffAngle = Math.abs(discAngle - DISC_UP);
		int diffVx = Math.abs(discVx - DISC_VX);
		int diffVy = Math.abs(discVy - DISC_VY);
		
		double reward = 0;
		double rewardAngle = 0;
		double rewardVx = 0;
		double rewardVy = 0;
		
		// get the rewards separately, and add them at the end
		
		switch(diffAngle) {
			case 0: // at the top
					rewardAngle = 200;
					break;
			case 1: //almost face up
					rewardAngle = 50;
					break;
			case 2: // a bit more far away from face up (typically, top right-right or top-left-left)
					rewardAngle = 30;
					break;
			case 3: // ^
					rewardAngle = 15;
					break;
			case 4: // ^
					rewardAngle = 5;
					break;
			case 5: // ^
					rewardAngle = 2;
					break;
			case 6: // ^
					rewardAngle = 1;
					break;
			}
		
		switch(diffVx) {
			case 0:
				rewardVx = 80;
				break;
			case 1:
				rewardVx = 30;
				break;
			case 2:
				rewardVx = 8;
				break;
			default:
				rewardVx = 3;
				break;
		}
		
		switch(diffVy) {
			case 0:
				rewardVy = 120;
				break;
			case 1:
				rewardVy = 60;
				break;
			case 2:
				rewardVy = 25;
				break;
			case 3: 
				rewardVy = 14;
				break;
			case 4:
				rewardVy = 8;
				break;
			case 5:
				rewardVy = 6;
				break;
			case 6: 
				rewardVy = 3;
				break;
			case 7:
				rewardVy = 1;
				break;
		}
		
		// sum everything
		reward = rewardAngle + rewardVx + rewardVy;
		// return sum
		return reward;
	}

	// ///////////////////////////////////////////////////////////
	// discretize() performs a uniform discretization of the
	// value parameter.
	// It returns an integer between 0 and nrValues-1.
	// The min and max parameters are used to specify the interval
	// for the discretization.
	// If the value is lower than min, 0 is returned
	// If the value is higher than min, nrValues-1 is returned
	// otherwise a value between 1 and nrValues-2 is returned.
	//
	// Use discretize2() if you want a discretization method that does
	// not handle values lower than min and higher than max.
	// ///////////////////////////////////////////////////////////
	public static int discretize(double value, int nrValues, double min,
			double max) {
		if (nrValues < 2) {
			return 0;
		}

		double diff = max - min;

		if (value < min) {
			return 0;
		}
		if (value > max) {
			return nrValues - 1;
		}

		double tempValue = value - min;
		double ratio = tempValue / diff;

		return (int) (ratio * (nrValues - 2)) + 1;
	}

	// ///////////////////////////////////////////////////////////
	// discretize2() performs a uniform discretization of the
	// value parameter.
	// It returns an integer between 0 and nrValues-1.
	// The min and max parameters are used to specify the interval
	// for the discretization.
	// If the value is lower than min, 0 is returned
	// If the value is higher than min, nrValues-1 is returned
	// otherwise a value between 0 and nrValues-1 is returned.
	// ///////////////////////////////////////////////////////////
	public static int discretize2(double value, int nrValues, double min,
			double max) {
		double diff = max - min;

		if (value < min) {
			return 0;
		}
		if (value > max) {
			return nrValues - 1;
		}

		double tempValue = value - min;
		double ratio = tempValue / diff;

		return (int) (ratio * nrValues);
	}

}
