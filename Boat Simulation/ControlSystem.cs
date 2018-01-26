using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ControlSystem : MonoBehaviour {

	public float centerx, centery;
	public float P;
	public float I;
	public float D;
	private PID controller;
	private Vector3 response;
	private Rigidbody rb;
	private float gravity = 9.8f;
	public float windSpeed;
	public float windDirection;
	private float backx;
	private float backy;
	private float backz;
	private float mag;
	private float angBoat;
	private float angForce;
	public float maxAng;
	public float maxForce;
	private float idealDrag;
	public float forceAttenuation;
	public Transform marker;
	private int powerOn = 1;

	// Use this for initialization
	void Start () {
		rb = GetComponents<Rigidbody>()[0];
		controller = new PID(P, I, D);
		marker.Translate (centerx, 0, -centery);

		// set terminal velocity
		idealDrag = 5.0f / 10.0f;
		rb.drag = idealDrag / ( idealDrag * Time.fixedDeltaTime + 1 );
	}
	
	// Update is called once per frame
	void Update () {
		// get control system response
		response = controller.Update (new Vector3(centerx, centery, 0), transform.position, Time.deltaTime);

		// apply constant forces
		rb.AddForce (new Vector3 (0, 0, gravity));
		rb.AddForce (new Vector3(windSpeed * Mathf.Cos(windDirection * Mathf.PI / 180), windSpeed * Mathf.Sin(windDirection * Mathf.PI / 180)));
		
		// find back of the boat
		backx = transform.position.x + Mathf.Sin (transform.eulerAngles.z * Mathf.PI / 180);
		backy = transform.position.y - Mathf.Cos (transform.eulerAngles.z * Mathf.PI / 180);
		backz = transform.position.z;

		// compute force angle and magnitude
		mag = response.magnitude;
		angBoat = (transform.eulerAngles.z + 90.0f) % 360.0f;
		if (angBoat < 0)
			angBoat += 360.0f;
		angForce = Mathf.Atan2 (response.y, response.x) * 180 / Mathf.PI;
		if (angForce < 0)
			angForce += 360.0f;
		angForce = (2 * angBoat - angForce) % 360.0f;
		if (angForce < 0)
			angForce += 360.0f;
		if (angForce - angBoat > maxAng && angForce - angBoat <= 180)
			angForce = angBoat + maxAng;
		if (angBoat - angForce > maxAng && angBoat - angForce < 180)
			angForce = angBoat - maxAng;

		mag = (mag / (1 + forceAttenuation * Mathf.Abs(angForce - angBoat))) * powerOn;
		// apply force
		rb.AddForceAtPosition (new Vector3 (Mathf.Min(mag, maxForce) * Mathf.Cos(angForce * Mathf.PI / 180), Mathf.Min(mag, maxForce) * Mathf.Sin(angForce * Mathf.PI / 180)), new Vector3 (backx, backy, backz));
	}

	void OnTriggerEnter(Collider other) {
		if (other.gameObject.CompareTag ("Target")) {
			powerOn = 0;
		}
	}

	void OnTriggerExit(Collider other) {
		if (other.gameObject.CompareTag ("Target")) {
			powerOn = 1;
		}
	}
}
